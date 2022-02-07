
#include "Clove.h"
#include "../vendor/ImGui/imgui.h"

class TestLayer : public Clove::Layer {
public:
	TestLayer() 
		: Layer(), m_camera_control(16.0f/9.0f, true)
	{

		// TEST SQUARE
		float vertices[(3 + 2) * 4] = {
			// xyz,               tex coords
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f
		};

		// vertex buffer
		Clove::Ref<Clove::VertexBuffer> m_vbo;
		m_vbo.reset(Clove::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_vbo->Bind();
		m_vbo->SetLayout({
			{ Clove::ShaderDataType::Float3, "a_pos" },
			{ Clove::ShaderDataType::Float2, "a_tex_coord" },
			});

		// index buffer
		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
		Clove::Ref<Clove::IndexBuffer> m_ibo;
		m_ibo.reset(Clove::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		// vertex array
		m_vao.reset(Clove::VertexArray::Create());
		m_vao->Bind();
		m_vao->SetIndexBuffer(m_ibo);
		m_vao->AddVertexBuffer(m_vbo);

		// shader
		m_shaderlib.Load("TextureShader", "../assets/shaders/texture_mvp.glsl");

		// texture
		m_texture = Clove::Texture2D::Create("../assets/checkerboard.png");
		m_texture2 = Clove::Texture2D::Create("../assets/cursor.png");

		m_shaderlib.Get("TextureShader")->Bind();
		m_shaderlib.Get("TextureShader")->SetUniform1i("u_texture", 0); // slot to sample from
	}

	void OnUpdate(float dt) override {

		m_camera_control.OnUpdate(dt);

		Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Clove::RenderCommand::Clear();

		Clove::Renderer::BeginScene(m_camera_control.GetCamera());
		
		m_texture->Bind();
		glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f,5.0f,1.0f));
		Clove::Renderer::Submit(m_vao, m_shaderlib.Get("TextureShader"), transform);
		
		m_texture2->Bind();
		glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), m_camera_control.GetCamera().GetPosition());
		Clove::Renderer::Submit(m_vao, m_shaderlib.Get("TextureShader"), transform2);
		
		Clove::Renderer::EndScene();
	}

	void OnImGuiRender() override {

	}

	void OnEvent(Clove::Event& e) override {

		m_camera_control.OnEvent(e);

		if (e.GetEventType() == Clove::EventType::WindowResize) {
			auto& wr = (Clove::WindowResizeEvent&)e;
		}

		// Event dispatch example
		Clove::EventDispatcher dp(e);
		dp.Dispatch<Clove::KeyPressEvent>(CLOVE_BIND_METHOD_1(TestLayer::OnKeyPressedEvent));
		if (e.GetEventType() == Clove::EventType::KeyPressed) {
			int key = dynamic_cast<Clove::KeyPressEvent&>(e).GetKeyCode();
			if (key > 0 && key < CHAR_MAX) {
				std::cout << (char)key;
			}
		}
	}

	bool OnKeyPressedEvent(Clove::KeyPressEvent& event) {
		return false;
	}

private:
	Clove::ShaderLibrary m_shaderlib;
	Clove::Ref<Clove::VertexArray> m_vao;
	Clove::CameraController m_camera_control;

	Clove::Ref<Clove::Texture2D> m_texture, m_texture2;
};




class GameTest : public Clove::GameApp {
public:
	GameTest() {
		GameApp::PushLayer( new TestLayer() ); // GameApp method
	}

	~GameTest() {

	}
};

Clove::GameApp* Clove::CreateApp() {
	return new GameTest();
}


