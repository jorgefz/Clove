#pragma once
#include "clovepch.h"

#include "Clove/Events/Event.h"
#include "Clove/Core/Layer.h"
#include "Clove/ImGui/ImGuiLayer.h"
#include "Clove/Core/Window.h"

#include "Clove/Renderer/Shader.h"
#include "Clove/Renderer/Buffer.h"
#include "Clove/Renderer/VertexArray.h"
#include "Clove/Renderer/Camera.h"

namespace Clove {

	class GameApp {
	public:
		GameApp();
		virtual ~GameApp();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer) { m_layer_stack.PushLayer(layer); }
		void PushOverlay(Layer* overlay) { m_layer_stack.PushOverlay(overlay); }

		Window& GetWindow() { return *m_window; }
		static GameApp& GetInstance() { return *m_instance; }

	private: // Event methods
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_window = nullptr;
		ImGuiLayer* m_imgui_layer = nullptr;
		bool m_running = true, m_minimised = false;
		LayerStack m_layer_stack;
		float m_frame_time = 0.0f;

	private:
		static GameApp* m_instance;
	};

	Clove::GameApp* CreateApp();

}