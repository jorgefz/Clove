#include "clovepch.h"
#include "Clove/Core/Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameApp.h"
#include "CLove/Renderer/Buffer.h"
#include "CLove/Renderer/VertexArray.h"
#include "CLove/Renderer/Renderer.h"
#include "CLove/Renderer/RenderCommand.h"

namespace Clove {

	// singleton instance
	GameApp* GameApp::m_instance = nullptr;

	GameApp::GameApp() {

		CLOVE_ASSERT(!m_instance, "Application already exists");
		m_instance = this;
		m_window = std::unique_ptr<Window>( Window::Create(1280, 720) );
		m_window->SetEventCallback(CLOVE_BIND_METHOD_1(GameApp::OnEvent));

		Renderer::Init();

		m_imgui_layer = new ImGuiLayer();
		m_layer_stack.PushOverlay(m_imgui_layer);
	}

	void GameApp::Run() {
		while (m_running) {

			float time = (float)glfwGetTime();
			float dt = time - m_frame_time;

			// update layers forward
			if (!m_minimised) {
				for (Layer* layer : m_layer_stack) layer->OnUpdate(dt);
			}

			m_imgui_layer->Begin();
			for (Layer* layer : m_layer_stack) layer->OnImGuiRender();
			m_imgui_layer->End();

			m_window->Update();
			
			m_frame_time = time;
		}
		for (Layer* layer : m_layer_stack) layer->OnDetach();
	}


	GameApp::~GameApp() {
		glfwSetErrorCallback(nullptr); // otherwise, glfwTerminate throws an error
		m_window->Destroy();
	}

	void GameApp::OnEvent(Event& e) {
		//std::cout << e.GetDebugName() << std::endl;

		EventDispatcher dp(e);
		dp.Dispatch<WindowCloseEvent>(CLOVE_BIND_METHOD_1(GameApp::OnWindowClose));
		dp.Dispatch<WindowResizeEvent>(CLOVE_BIND_METHOD_1(GameApp::OnWindowResize));

		// dispatch events to layers in reverse order (front layers receive it first)
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*(--it))->OnEvent(e);
			if (e.Handled()) break;
		}
	}

	// Event methods
	bool GameApp::OnWindowClose(WindowCloseEvent& e) {
		m_running = false;
		return true; // event handled
	}

	bool GameApp::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_minimised = true;
			return false;
		}
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_minimised = false;
		return false;
	}

}