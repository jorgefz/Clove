
#include "Clove.h"
#include "../vendor/ImGui/imgui.h"

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
		Clove::Ref<Clove::VertexBuffer> m_vbo;
		m_vbo.reset(Clove::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_vbo->Bind();
		m_vbo->SetLayout({
			{ Clove::ShaderDataType::Float3, "a_pos" },
			{ Clove::ShaderDataType::Float4, "a_color" },
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
			"../resources/shaders/color_mvp.vert.glsl",
			"../resources/shaders/color_mvp.frag.glsl")
		);

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
		
		for (unsigned int i = 0; i != 15; ++i) {
			for (unsigned int j = 0; j != 15; ++j) {
				if (i % 2) m_shader->SetUniform4f("u_color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
				else       m_shader->SetUniform4f("u_color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) );
				if (j % 2 && i % 2) m_shader->SetUniform4f("u_color", glm::vec4(1.0f, 0.7f, 0.0f, 1.0f) );
				else if (j % 2) m_shader->SetUniform4f("u_color", glm::vec4(0.69f, 0.26f, 0.96f, 1.0f) );
				glm::mat4 transform(1.0f);
				glm::vec3 translate((float)i * 0.2f, (float)j * 0.2f, 0.0f);
				glm::vec3 scale(0.15f, 0.15f, 1.0f);
				transform = glm::scale(glm::translate(transform, translate), scale);
				Clove::Renderer::Submit(m_vao, m_shader, transform);
			}
		}

		m_shader->SetUniform4f("u_color", square_color);
		Clove::Renderer::Submit(m_vao, m_shader);

		Clove::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		
		float color[3] = { square_color.x, square_color.y, square_color.z };
		
		ImGui::Begin("Testing", 0, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::ColorEdit3("Color Edit", color);
		ImGui::End();

		square_color = glm::vec4(color[0], color[1], color[2], 1.0f);
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
	
	glm::vec4 square_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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


