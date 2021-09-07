#pragma once
#include "clovepch.h"

#include "Events/Event.h"
#include "Layer.h"
#include "Window.h"

namespace Clove {

	class GameApp {
	public:
		GameApp();
		virtual ~GameApp();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer) { m_layer_stack.PushLayer(layer); }
		void PushOverlay(Layer* overlay) { m_layer_stack.PushOverlay(overlay); }

	private: // Event methods
		bool OnWindowClose();

	private:
		std::unique_ptr<Clove::Window> m_window = nullptr;
		bool m_running = true;
		LayerStack m_layer_stack;
	};

	Clove::GameApp* CreateApp();

}
