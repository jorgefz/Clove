
#include "Clove.h"

class TestLayer : public Clove::Layer {
public:
	TestLayer() 
		: Layer(), m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		// TEST TRIANGLE
		float vertices[(3 + 4) * 4] = {
			// xyz,               rgba
			-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		};

		// vertex buffer
		std::shared_ptr<Clove::VertexBuffer> m_vbo;
		m_vbo.reset(Clove::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_vbo->Bind();
		m_vbo->SetLayout({
			{ Clove::ShaderDataType::Float3, "a_pos" },
			{ Clove::ShaderDataType::Float4, "a_color" },
			});

		// index buffer
		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Clove::IndexBuffer> m_ibo;
		m_ibo.reset(Clove::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		// vertex array
		m_vao.reset(Clove::VertexArray::Create());
		m_vao->Bind();
		m_vao->SetIndexBuffer(m_ibo);
		m_vao->AddVertexBuffer(m_vbo);

		// shader
		m_shader.reset(new Clove::Shader(
			"../resources/shaders/mvp.vert.glsl",
			"../resources/shaders/mvp.frag.glsl")
		);

	}

	void OnUpdate() override {

		// BEGIN Camera movement test
		glm::vec3 cam_pos = m_camera.GetPosition();
		float cam_rot = m_camera.GetRotation();
		float cam_speed = 0.05f;
		float cam_rot_rate = 1.0f;
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_W)) cam_pos.y -= cam_speed;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_S)) cam_pos.y += cam_speed;
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_D)) cam_pos.x -= cam_speed;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_A)) cam_pos.x += cam_speed;
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_R)) cam_rot -= cam_rot_rate;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_F)) cam_rot += cam_rot_rate;
		m_camera.SetPosition(cam_pos);
		m_camera.SetRotation(cam_rot);
		// END Camera movement test

		Clove::RenderCommand::SetClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		Clove::RenderCommand::Clear();

		m_camera.Update();

		Clove::Renderer::BeginScene(m_camera);
		Clove::Renderer::Submit(m_vao, m_shader);
		Clove::Renderer::EndScene();
	}

	void OnImGuiRender() override {
	
	}

	void OnEvent(Clove::Event& e) override {
		
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
	std::shared_ptr<Clove::Shader> m_shader;
	std::shared_ptr<Clove::VertexArray> m_vao;
	Clove::Camera m_camera;
};

class GameTest : public Clove::GameApp {
public:
	GameTest() {
		PushLayer( new TestLayer() ); // GameApp method
	}

	~GameTest() {

	}
};

Clove::GameApp* Clove::CreateApp() {
	return new GameTest();
}