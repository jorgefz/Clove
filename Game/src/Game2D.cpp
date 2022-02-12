

#include "Game2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <chrono>

//#include "Clove/Debug/Instrumentor.h"


Game2D::Game2D()
	: Layer("Game2D"), m_camera_control(16.0f/9.0f)
{

}


void Game2D::OnAttach() {
	CLOVE_PROFILE_FUNCTION();
	m_TextureChess = Clove::Texture2D::Create("assets/checkerboard.png");
}


void Game2D::OnDetach() {
	CLOVE_PROFILE_FUNCTION();
	Clove::Renderer2D::Shutdown();
}

void Game2D::OnUpdate(float dt) {

	m_camera_control.OnUpdate(dt);

	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	Clove::Renderer2D::DrawQuad({0.0f,0.0f}, {0.5f,0.5f}, {0.9f, 0.0f, 0.5f, 1.0f});
	Clove::Renderer2D::DrawQuad({-1.0f,1.0f}, {2.0f,1.0f},{0.9f, 0.7f, 0.1f, 1.0f});
	Clove::Renderer2D::DrawQuad({ -5.0f,-5.0f,-0.1f}, { 21.0f,21.0f }, m_TextureChess, 5.0f);

	Clove::QuadProperties props{};
	props.position = glm::vec3(2.0f, -1.0f, 0.9f);
	props.color = glm::vec4{ 0.0f,0.8f,0.5f,1.0f };
	props.texture = m_TextureChess;
	props.rotation = glm::radians(45.0f);
	props.tiling_factor = 1.5f;
	Clove::Renderer2D::DrawQuad(props);

	// This would be valid C++20, but switching version breaks other stuff:
	//Clove::Renderer2D::DrawQuad({ .color = glm::vec4{0.5f} });

	Clove::Renderer2D::EndScene();

}

void Game2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
	ImGui::End();
}

void Game2D::OnEvent(Clove::Event& e) {
	m_camera_control.OnEvent(e);
}


