#include "clovepch.h"
#include "Core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameApp.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

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


		// TEST TRIANGLE
		float vertices[ (3 + 4) * 4 ] = {
			// xyz,               rgba
			-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		};

		// vertex buffer
		std::shared_ptr<VertexBuffer> m_vbo;
		m_vbo.reset( VertexBuffer::Create(vertices, sizeof(vertices)) );
		m_vbo->Bind();
		m_vbo->SetLayout({
			{ ShaderDataType::Float3, "a_pos" },
			{ ShaderDataType::Float4, "a_color" },
		});
		
		// index buffer
		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> m_ibo;
		m_ibo.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int) ));

		// vertex array
		m_vao.reset(VertexArray::Create());
		m_vao->Bind();
		m_vao->SetIndexBuffer( m_ibo );
		m_vao->AddVertexBuffer(m_vbo);

		// shader
		m_shader.reset( new Shader("../resources/shaders/basic.vert.glsl", "../resources/shaders/basic.frag.glsl") );
	}

	void GameApp::Run() {
		while (m_running) {
			RenderCommand::SetClearColor(1.0f, 0.0f, 1.0f, 1.0f);
			RenderCommand::Clear();

			Renderer::BeginScene();
			m_shader->Bind();
			Renderer::Submit(m_vao);
			Renderer::EndScene();

			// update layers forward
			for (Layer* layer : m_layer_stack) layer->OnUpdate();

			m_imgui_layer->Begin();
			for (Layer* layer : m_layer_stack) layer->OnImGuiRender();
			m_imgui_layer->End();

			m_window->Update();
		}
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

	// Event methods
	bool GameApp::OnWindowClose() {
		m_running = false;
		return true; // event handled
	}

}