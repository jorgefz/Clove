#include "clovepch.h"
#include "GameApp.h"

namespace Clove {

	GameApp::GameApp() {
		m_window = std::unique_ptr<Clove::Window>( new Clove::Window );
		m_window->Create(1280, 720);
		// When event occurs, OnEvent is called and the event is passed to it.
		// placeholder::1 is the Event& e
		m_window->SetEventCallback( std::bind(&GameApp::OnEvent, this, std::placeholders::_1) );
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
			// update layers forward
			for (Layer* layer : m_layer_stack) layer->OnUpdate();
			m_window->Update();
		}
	}

	// Event methods
	bool GameApp::OnWindowClose() {
		m_running = false;
		return true; // event handled
	}

}