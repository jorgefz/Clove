#include "clovepch.h"
#include "Clove/Core.h"
#include "GameApp.h"

namespace Clove {

	// singleton instance
	GameApp* GameApp::m_instance = nullptr;

	GameApp::GameApp() {

		if (m_instance) throw std::runtime_error("Application already exists!\n");
		m_instance = this;

		m_window = std::unique_ptr<Clove::Window>( new Clove::Window );
		m_window->Create(1280, 720);
		// When event occurs, OnEvent is called and the event is passed to it.
		// placeholder::1 is the Event& e
		m_window->SetEventCallback( std::bind(&GameApp::OnEvent, this, std::placeholders::_1) );
		
		m_imgui_layer = new ImGuiLayer();
		m_layer_stack.PushOverlay(m_imgui_layer);
	}

	GameApp::~GameApp() {
		glfwSetErrorCallback(nullptr); // otherwise, glfwTerminate throws an error
		m_window->Destroy();
	}

	void GameApp::OnEvent(Event& e) {		
		//std::cout << e.GetDebugName() << std::endl;

		EventDispatcher dp(e);
		auto f = std::bind(&GameApp::OnWindowClose, this);
		dp.Dispatch<WindowCloseEvent>(f);

		// dispatch events to layers in reverse order (front layers receive it first)
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*(--it))->OnEvent(e);
			if (e.Handled()) break;
		}
	}

	void GameApp::Run() {
		//while (!m_window->ShouldClose()) {
		while (m_running) {
			glClear(GL_COLOR_BUFFER_BIT);
			// update layers forward
			for (Layer* layer : m_layer_stack) layer->OnUpdate();

			m_imgui_layer->Begin();
			for (Layer* layer : m_layer_stack) layer->OnImGuiRender();
			m_imgui_layer->End();

			m_window->Update();
		}
	}

	// Event methods
	bool GameApp::OnWindowClose() {
		m_running = false;
		return true; // event handled
	}

}