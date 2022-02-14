
#include "TestLayer.h"

TestLayer::TestLayer()
	: Layer("TestLayer"), m_camera_control(16.0f / 9.0f)
{  }

void TestLayer::OnAttach() {
	// m_GrassTexture = Clove::Texture2D::Create();
	// m_RockTexture = Clove::Texture2D::Create();
	// m_PlayerTexture = Clove::Texture2D::Create();
}

void TestLayer::OnDetach() {
	Clove::Renderer2D::Shutdown();
}


void TestLayer::OnUpdate(float dt) {

	m_camera_control.OnUpdate(dt);

	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	// Clove::Renderer2D::DrawQuad();
	Clove::Renderer2D::EndScene();
}

void TestLayer::OnImGuiRender() {

}

void TestLayer::OnEvent(Clove::Event& e) {

	m_camera_control.OnEvent(e);

	Clove::EventDispatcher dp(e);
	dp.Dispatch<Clove::KeyPressEvent>(CLOVE_BIND_METHOD_1(TestLayer::OnKeyPressedEvent));
}

bool TestLayer::OnKeyPressedEvent(Clove::KeyPressEvent& event) {
	return false;
}
