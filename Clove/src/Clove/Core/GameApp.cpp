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

	GameApp::GameApp(const std::string title) {

		CLOVE_PROFILE_FUNCTION();

		CLOVE_ASSERT(!m_instance, "Clove application already exists");
		m_instance = this;

		WindowData data{};
		data.width = 1280;
		data.height = 720;
		data.title = title;
		m_window = Window::Create(data);
		m_window->SetEventCallback(CLOVE_BIND_METHOD_1(GameApp::OnEvent));

		Renderer::Init();

		m_imgui_layer = new ImGuiLayer();
		m_layer_stack.PushOverlay(m_imgui_layer);
		m_imgui_layer->OnAttach();
	}

	void GameApp::PushLayer(Layer* layer) {
		CLOVE_PROFILE_FUNCTION();
		m_layer_stack.PushLayer(layer);
		layer->OnAttach();
	}

	void GameApp::PushOverlay(Layer* overlay) {
		CLOVE_PROFILE_FUNCTION();
		m_layer_stack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void GameApp::Run() {
		CLOVE_PROFILE_FUNCTION();

		while (m_running) {
			CLOVE_PROFILE_SCOPE("Main Game Loop");

			float time = (float)glfwGetTime();
			float dt = time - m_frame_time;

			// update layers forward
			if (!m_minimised) {
				{
					CLOVE_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_layer_stack) layer->OnUpdate(dt);
				}
			}

			m_imgui_layer->Begin();
			{
				CLOVE_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer* layer : m_layer_stack) layer->OnImGuiRender();
			}
			m_imgui_layer->End();
			m_window->Update();
			m_frame_time = time;
		}
		for (Layer* layer : m_layer_stack) layer->OnDetach();
	}

	void GameApp::Close() {
		m_running = false;
	}


	GameApp::~GameApp() {
		CLOVE_PROFILE_FUNCTION();
		glfwSetErrorCallback(nullptr); // otherwise, glfwTerminate throws an error
		m_window->Destroy();
	}

	void GameApp::OnEvent(Event& e) {
		CLOVE_PROFILE_FUNCTION();

		EventDispatcher dp(e);
		dp.Dispatch<WindowCloseEvent>(CLOVE_BIND_METHOD_1(GameApp::OnWindowClose));
		dp.Dispatch<WindowResizeEvent>(CLOVE_BIND_METHOD_1(GameApp::OnWindowResize));

		// dispatch events to layers in reverse order (front layers receive it first)
		for (auto it = m_layer_stack.rbegin(); it != m_layer_stack.rend(); it++) {
			if (e.Handled()) break;
			(*it)->OnEvent(e);
		}
	}

	// Event methods
	bool GameApp::OnWindowClose(WindowCloseEvent& e) {
		m_running = false;
		return true; // event handled
	}

	bool GameApp::OnWindowResize(WindowResizeEvent& e) {
		CLOVE_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_minimised = true;
			return false;
		}
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_minimised = false;
		return false;
	}

}