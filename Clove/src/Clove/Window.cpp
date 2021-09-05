
#include "Window.h"

namespace Clove {

	// Forward declaration for calling this from window creator
	void SetGLFWCallbacks(GLFWwindow* window);

	void GLFWErrorCallback(int error, const char* description) {
		std::cout << "GLFW Error " << error << ": " << description << std::endl;
		throw std::runtime_error(" ");
	}

	Window::Window()
		: m_window_ptr(nullptr),
		m_initialised(false) { }

	Window::~Window() { 
		Window::Destroy();
	}


	void Window::Create(GLuint width, GLuint height) {

		m_data.width = width;
		m_data.height = height;
		m_data.title = "Clove";
		m_data.event_callback = nullptr;

		GLFWmonitor* monitor = nullptr;
		bool fullscreen = false;

		if (!glfwInit()) {
			throw std::runtime_error("[GLFW] Fatal error: failed to load!");
		}
		glfwSetErrorCallback(GLFWErrorCallback);

		if (fullscreen) monitor = glfwGetPrimaryMonitor();
		m_window_ptr = glfwCreateWindow(width, height, m_data.title.c_str(), monitor, nullptr);
		if (!m_window_ptr) {
			glfwTerminate();
			throw std::runtime_error("[GLFW] Fatal error: failed to create window");
		}

		glfwMakeContextCurrent(m_window_ptr);
		glfwSwapInterval(1); // activate VSYNC, problems with NVIDIA cards
		glViewport(0, 0, width, height);
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			glfwDestroyWindow(m_window_ptr);
			throw std::runtime_error("[GLEW] Fatal error: failed to load!");
		}
		
		#ifdef _DEBUG
		int glfw_major, glfw_minor, glfw_rev;
		glfwGetVersion(&glfw_major, &glfw_minor, &glfw_rev);
		std::cout << "[GLEW] Version " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "[GLFW] Version " << glfw_major << '.' << glfw_minor << '.' << glfw_rev << std::endl;
		#endif

		m_initialised = true;
		glfwSetWindowUserPointer(m_window_ptr, &m_data);

		// Adds up alpha channels
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Clove::SetGLFWCallbacks(m_window_ptr);
	}

	GLFWwindow* Window::GetWindow() {
		return m_window_ptr;
	}

	void Window::Update() {
		glfwSwapBuffers(m_window_ptr);
		glfwPollEvents();
	}

	void Window::Destroy() {
		if (m_initialised) glfwTerminate();
		if (m_window_ptr) glfwDestroyWindow(m_window_ptr);
		m_initialised = false;
		m_window_ptr = nullptr;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(m_window_ptr);
	}



	//===== GLFW Callback Functions =====//

	static void WindowResizeCallback(GLFWwindow* w, int width, int height) {
		glViewport(0, 0, width, height);
		Clove::WindowData* data = static_cast<Clove::WindowData*>(glfwGetWindowUserPointer(w));
		data->width = static_cast<GLuint>(width);
		data->height = static_cast<GLuint>(height);

		WindowResizeEvent e(width, height);
		data->event_callback(e);
	}

	static void WindowCloseCallback(GLFWwindow* w) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		WindowCloseEvent e;
		data->event_callback(e);
	}

	static void KeyCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		switch (action) {
			case GLFW_PRESS: {
				KeyPressEvent e(key, 0);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleaseEvent e(key);
				data->event_callback(e);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressEvent e(key, 1);
				data->event_callback(e);
				break;
			}
		}
	}

	static void MouseMovedCallback(GLFWwindow* w, double xpos, double ypos) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		MouseMovedEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
		data->event_callback(e);
	}

	static void MouseScrolledCallback(GLFWwindow* w, double xoffset, double yoffset) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		MouseScrolledEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
		data->event_callback(e);
	}

	static void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		
		switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent e(button);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent e(button);
				data->event_callback(e);
				break;
			}
		}
	}


	void SetGLFWCallbacks(GLFWwindow* window) {
		glfwSetFramebufferSizeCallback(window, WindowResizeCallback);
		glfwSetWindowCloseCallback(window, WindowCloseCallback);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetScrollCallback(window, MouseScrolledCallback);
		glfwSetCursorPosCallback(window, MouseMovedCallback);
	}

	
}
