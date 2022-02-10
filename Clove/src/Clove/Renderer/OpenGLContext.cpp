#include "clovepch.h"
#include "Clove/Core/Core.h"
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
		CLOVE_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_window_handle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CLOVE_ASSERT(status, "Failed to load GLAD!");
		CLOVE_INFO("GPU: %s", glGetString(GL_RENDERER));
		CLOVE_INFO("Vendor: %s", glGetString(GL_VENDOR));
		CLOVE_INFO("OpenGL Version %s", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		CLOVE_PROFILE_FUNCTION();
		glfwSwapBuffers(m_window_handle);
	}
}