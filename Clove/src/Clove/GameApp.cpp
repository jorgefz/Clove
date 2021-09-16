#include "clovepch.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		//!! Test triangle
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.7f, 0.0f
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);

		glGenBuffers(1, &m_ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
		unsigned int indices[] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// SHADER
		m_shader.reset( new Shader("../resources/shaders/basic.vert.glsl", "../resources/shaders/basic.frag.glsl") );
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
			glClearColor(25.0f / 255.0f, 11.0f / 255.0f, 102.0f / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//!! Test Triangle
			m_shader->Bind();
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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