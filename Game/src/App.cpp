
#include "Clove.h"
#include "../vendor/ImGui/imgui.h"

class TestLayer : public Clove::Layer {
public:
	TestLayer() 
		: Layer(), m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
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
		m_shader.reset( Clove::Shader::Create(
			"../assets/shaders/texture_mvp.vert.glsl",
			"../assets/shaders/texture_mvp.frag.glsl"
		));

		// texture
		m_texture = Clove::Texture2D::Create("../assets/checkerboard.png");
		m_texture2 = Clove::Texture2D::Create("../assets/cursor.png");

		m_shader->Bind();
		m_shader->SetUniform1i("u_texture", 0); // slot to sample from
	}

	void OnUpdate(float dt) override {

		// BEGIN Camera movement test
		glm::vec3 cam_pos = m_camera.GetPosition();
		float cam_rot = m_camera.GetRotation();
		float cam_speed = 1.0f * dt; // 1 units per second 
		float cam_rot_rate = 45.0f * dt; // 45 degrees per second
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_W)) cam_pos.y += cam_speed;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_S)) cam_pos.y -= cam_speed;
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_D)) cam_pos.x += cam_speed;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_A)) cam_pos.x -= cam_speed;
		if (Clove::Input::IsKeyPressed(Clove::Key::KEY_R)) cam_rot += cam_rot_rate;
		else if (Clove::Input::IsKeyPressed(Clove::Key::KEY_F)) cam_rot -= cam_rot_rate;
		
		if      (cam_rot > 360.0f) cam_rot -= 360.0f;
		else if (cam_rot < 0.0f)   cam_rot += 360.0f;
		
		m_camera.SetPosition(cam_pos);
		m_camera.SetRotation(cam_rot);
		// END Camera movement test

		Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Clove::RenderCommand::Clear();

		m_camera.Update();

		Clove::Renderer::BeginScene(m_camera);
		m_shader->Bind();
		m_texture->Bind();
		Clove::Renderer::Submit(m_vao, m_shader);
		m_texture2->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), cam_pos);
		Clove::Renderer::Submit(m_vao, m_shader, transform);
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
	Clove::Ref<Clove::Shader> m_shader;
	Clove::Ref<Clove::VertexArray> m_vao;
	Clove::Camera m_camera;

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


