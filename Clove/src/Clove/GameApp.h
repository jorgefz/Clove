#pragma once
#include "clovepch.h"

#include "Events/Event.h"
#include "Layer.h"
#include "Clove/ImGui/ImGuiLayer.h"
#include "Window.h"

#include "Shader.h"

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
		bool OnWindowClose();

	private:
		std::unique_ptr<Window> m_window = nullptr;
		ImGuiLayer* m_imgui_layer = nullptr;
		bool m_running = true;
		LayerStack m_layer_stack;
		
		unsigned int m_vao, m_vbo, m_ib; // Temporary
		std::unique_ptr<Shader> m_shader;

	private:
		static GameApp* m_instance;
	};

	Clove::GameApp* CreateApp();

}
