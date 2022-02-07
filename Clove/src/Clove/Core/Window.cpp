#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Clove/Core/Window.h"
#include "Clove/Renderer/OpenGLContext.h"

namespace Clove {

	// Forward declaration for calling this from window creator
	void SetGLFWCallbacks(GLFWwindow* window);

	void GLFWErrorCallback(int error, const char* description) {
		std::cout << "GLFW Error " << error << ": " << description << std::endl;
		CLOVE_ASSERT(false, "Fatal Error");
	}

	Window::Window(unsigned int width, unsigned int height)
		: m_window_ptr(nullptr),
		m_initialised(false),
		m_context(nullptr)
	{
		m_data.width = width;
		m_data.height = height;
		m_data.title = "Clove Game";
		m_data.event_callback = nullptr;

		CLOVE_ASSERT(glfwInit(), "[GLFW] Fatal error: failed to load!");
		glfwSetErrorCallback(GLFWErrorCallback);

		m_window_ptr = glfwCreateWindow(width, height, m_data.title.c_str(), nullptr, nullptr);
		if (!m_window_ptr) {
			glfwTerminate();
			throw std::runtime_error("[GLFW] Fatal error: failed to create window");
		}

		m_context = new OpenGLContext((GLFWwindow*)m_window_ptr);
		m_context->Init();

		glfwSwapInterval(1); // activate VSYNC, problems with NVIDIA cards
		glViewport(0, 0, width, height);

		#ifdef CLOVE_DEBUG
		int glfw_major, glfw_minor, glfw_rev;
		glfwGetVersion(&glfw_major, &glfw_minor, &glfw_rev);
		std::cout << "[GLAD] Version " << GL_VERSION << std::endl;
		std::cout << "[GLFW] Version " << glfw_major << '.' << glfw_minor << '.' << glfw_rev << std::endl;
		#endif

		m_initialised = true;
		glfwSetWindowUserPointer((GLFWwindow*)m_window_ptr, &m_data);

		Clove::SetGLFWCallbacks((GLFWwindow*)m_window_ptr);
	}

	Window::~Window() { 
		Window::Destroy();
	}


	Window* Window::Create(unsigned int width, unsigned int height) {
		return new Window(width, height);
	}

	void* Window::GetHandle() {
		return m_window_ptr;
	}

	void Window::Update() {
		glfwSwapBuffers((GLFWwindow*)m_window_ptr);
		glfwPollEvents();
	}

	void Window::Destroy() {
		if (m_initialised) glfwTerminate();
		if (m_window_ptr) glfwDestroyWindow((GLFWwindow*)m_window_ptr);
		m_initialised = false;
		m_window_ptr = nullptr;
		//delete m_context;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose((GLFWwindow*)m_window_ptr);
	}



	//===== GLFW Callback Functions =====//

	static void WindowResizeCallback(GLFWwindow* w, int width, int height) {
		Clove::WindowData* data = static_cast<Clove::WindowData*>(glfwGetWindowUserPointer(w));
		data->width = static_cast<unsigned int>(width);
		data->height = static_cast<unsigned int>(height);

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
			default:
				break;
		}
	}

	static void CharCallback(GLFWwindow* w, unsigned int key_typed) {
		Clove::WindowData* data = (Clove::WindowData*)(glfwGetWindowUserPointer(w));
		KeyTypeEvent e(key_typed);
		data->event_callback(e);
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
		glfwSetCharCallback(window, CharCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetScrollCallback(window, MouseScrolledCallback);
		glfwSetCursorPosCallback(window, MouseMovedCallback);
	}

	
}
