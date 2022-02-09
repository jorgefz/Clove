#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Clove/Core/GameApp.h"

namespace Clove {

	// Singleton instance
	Input* Input::m_instance = new Input();

	bool Input::IsKeyPressed(int keycode) {
		GLFWwindow* window = (GLFWwindow*)GameApp::GetInstance().GetWindow().GetHandle();
		auto state = glfwGetKey(window, keycode);
		return (state == GLFW_PRESS || state == GLFW_REPEAT);
	}

	bool Input::IsMouseButtonPressed(int button) {
		GLFWwindow* window = (GLFWwindow*)GameApp::GetInstance().GetWindow().GetHandle();
		auto state = glfwGetMouseButton(window, button);
		return (state == GLFW_PRESS);
	}
	std::pair<float,float> Input::GetMousePos() {
		GLFWwindow* window = (GLFWwindow*)GameApp::GetInstance().GetWindow().GetHandle();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}
	float Input::GetMouseX() {
		return Input::GetMousePos().first;
	}
	float Input::GetMouseY() {
		return Input::GetMousePos().second;
	}
}