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
		CLOVE_ASSERT(false, "GLFW Error %d: %s", error, description);
	}


	Window::Window(const WindowData& data)
		: m_window_ptr(nullptr),
		m_initialised(false),
		m_context(nullptr),
		m_data(data)
	{
		CLOVE_PROFILE_FUNCTION();

		{
			CLOVE_PROFILE_SCOPE("glfwInit");
			CLOVE_ASSERT(glfwInit(), "Failed to load GLFW!");
		}
		glfwSetErrorCallback(GLFWErrorCallback);

		{
			CLOVE_PROFILE_SCOPE("glfwCreateWindow");
			m_window_ptr = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		}

		if (!m_window_ptr) {
			glfwTerminate();
			CLOVE_ASSERT(false, "Failed to create GLFW window!");
		}

		m_context = CreateRef<OpenGLContext>(m_window_ptr);
		m_context->Init();

		Window::SetVSync(true); // activate VSYNC, problems with NVIDIA cards
		glViewport(0, 0, m_data.width, m_data.height);

		int glfw_major, glfw_minor, glfw_rev;
		glfwGetVersion(&glfw_major, &glfw_minor, &glfw_rev);
		CLOVE_INFO("GLFW Version %d.%d.%d", glfw_major, glfw_minor, glfw_rev);

		m_initialised = true;
		glfwSetWindowUserPointer((GLFWwindow*)m_window_ptr, &m_data);

		Clove::SetGLFWCallbacks((GLFWwindow*)m_window_ptr);
	}

	Window::~Window() {
		CLOVE_PROFILE_FUNCTION();
		Window::Destroy();
	}


	Scope<Window> Window::Create(uint32_t width, uint32_t height) {
		WindowData data{};
		data.height = width;
		data.height = height;
		return CreateScope<Window>(data);
	}

	Scope<Window> Window::Create(const WindowData& data) {
		return CreateScope<Window>(data);
	}

	void* Window::GetHandle() {
		return m_window_ptr;
	}

	void Window::Update() {
		CLOVE_PROFILE_FUNCTION();
		m_context->SwapBuffers();
		glfwPollEvents();
	}

	void Window::Destroy() {
		CLOVE_PROFILE_FUNCTION();
		if (m_initialised) glfwTerminate();
		if (m_window_ptr) glfwDestroyWindow((GLFWwindow*)m_window_ptr);
		m_initialised = false;
		m_window_ptr = nullptr;
		//delete m_context;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose((GLFWwindow*)m_window_ptr);
	}

	void Window::SetVSync(bool enabled) {
		CLOVE_PROFILE_FUNCTION();
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
	}



	//===== GLFW Callback Functions =====//

	static void WindowResizeCallback(GLFWwindow* w, int width, int height) {
		Clove::WindowData* data = static_cast<Clove::WindowData*>(glfwGetWindowUserPointer(w));
		data->width = static_cast<uint32_t>(width);
		data->height = static_cast<uint32_t>(height);

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
