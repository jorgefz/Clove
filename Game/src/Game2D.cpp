

#include "Game2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <chrono>

#include "Clove/Debug/Instrumentor.h"


Game2D::Game2D()
	: Layer("Game2D"), m_camera_control(16.0f/9.0f)
{

}


void Game2D::OnAttach() {
	m_TextureChess = Clove::Texture2D::Create("assets/checkerboard.png");
}


void Game2D::OnDetach() {
	Clove::Renderer2D::Shutdown();
}

void Game2D::OnUpdate(float dt) {

	Instrumentor::Get().BeginSession("TestSession");

	CLOVE_PROFILE_SCOPE("Game2D::OnUpdate");

	{
		CLOVE_PROFILE_SCOPE("CameraController::OnUpdate");
		m_camera_control.OnUpdate(dt);
	}

	{
		CLOVE_PROFILE_SCOPE("Render Prep");
		Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Clove::RenderCommand::Clear();
	}

	{
		CLOVE_PROFILE_SCOPE("Render Scene");
		Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
		Clove::Renderer2D::DrawQuad({0.0f,0.0f}, {0.5f,0.5f}, {0.9f, 0.0f, 0.5f, 1.0f});
		Clove::Renderer2D::DrawQuad({-1.0f,1.0f}, {2.0f,1.0f}, {0.9f, 0.7f, 0.1f, 1.0f});
		Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f}, { 21.0f,21.0f }, m_TextureChess);
		Clove::Renderer2D::EndScene();
	}

	Instrumentor::Get().EndSession();
}

void Game2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
	
	for (auto& p : m_profilers) {
		ImGui::Text("%.3f ms\t%s", p.time, p.name);
	}
	m_profilers.clear();
	
	ImGui::End();
}

void Game2D::OnEvent(Clove::Event& e) {
	m_camera_control.OnEvent(e);
}


