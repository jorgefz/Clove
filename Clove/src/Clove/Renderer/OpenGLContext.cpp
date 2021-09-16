#include "clovepch.h"
#include "Core.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "OpenGLContext.h"

namespace Clove {
	
	OpenGLContext::OpenGLContext(GLFWwindow* handle)
		: m_window_handle(handle)
	{
		CLOVE_ASSERT(handle, "[OpenGLContext] window handle is null!");
	}
	
	void OpenGLContext::Init(){
		glfwMakeContextCurrent(m_window_handle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CLOVE_ASSERT(status, "[GLAD] Fatal error: failed to load!");
		std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "[OpenGL] Version " << glGetString(GL_VERSION) << std::endl;
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_window_handle);
	}
}