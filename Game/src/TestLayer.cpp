
#include "TestLayer.h"

TestLayer::TestLayer()
	: Layer("TestLayer"), m_camera_control(16.0f / 9.0f, true)
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
	m_vbo = Clove::VertexBuffer::Create(vertices, sizeof(vertices));
	m_vbo->Bind();
	m_vbo->SetLayout({
		{ Clove::ShaderDataType::Float3, "a_pos" },
		{ Clove::ShaderDataType::Float2, "a_TexCoord" },
		});

	// index buffer
	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
	Clove::Ref<Clove::IndexBuffer> m_ibo;
	m_ibo = Clove::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

	// vertex array
	m_vao = Clove::VertexArray::Create();
	m_vao->Bind();
	m_vao->SetIndexBuffer(m_ibo);
	m_vao->AddVertexBuffer(m_vbo);

	// shader
	m_shaderlib.Load("TextureShader", "../Game/assets/shaders/Texture.glsl");

	// texture
	m_texture = Clove::Texture2D::Create("../Game/assets/checkerboard.png");
	m_texture2 = Clove::Texture2D::Create("../Game/assets/cursor.png");

	m_shaderlib.Get("TextureShader")->Bind();
	m_shaderlib.Get("TextureShader")->SetUniform1i("u_Texture", 0); // slot to sample from
}

void TestLayer::OnUpdate(float dt) {

	m_camera_control.OnUpdate(dt);

	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer::BeginScene(m_camera_control.GetCamera());

	m_texture->Bind();
	glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 1.0f));
	Clove::Renderer::Submit(m_vao, m_shaderlib.Get("TextureShader"), transform);

	m_texture2->Bind();
	glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), m_camera_control.GetCamera().GetPosition());
	Clove::Renderer::Submit(m_vao, m_shaderlib.Get("TextureShader"), transform2);

	Clove::Renderer::EndScene();
}

void TestLayer::OnImGuiRender() {

}

void TestLayer::OnEvent(Clove::Event& e) {

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

bool TestLayer::OnKeyPressedEvent(Clove::KeyPressEvent& event) {
	return false;
}
