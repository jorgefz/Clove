

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
	m_TextureRing = Clove::Texture2D::Create("assets/cursor.png");

	// particle system
	m_particle.color_start = { 254.0f/255.0f, 212.0f/255.0f, 123.0f/255.0f, 1.0f };
	m_particle.color_start = { 254.0f/255.0f, 109.0f/255.0f, 41.0f/255.0f, 1.0f };
	m_particle.size_start = 0.5f, m_particle.size_change = 0.3f, m_particle.size_end = 0.0f;
	m_particle.lifetime = 1.0f;
	m_particle.vel = { 0.0f,0.0f }, m_particle.acc = { 3.0f,1.0f }, m_particle.pos = { 0.0f,0.0f };
}


void Game2D::OnDetach() {
	CLOVE_PROFILE_FUNCTION();
	Clove::Renderer2D::Shutdown();
}

void Game2D::OnUpdate(float dt) {

	m_fps = 1.0f / dt;

	m_camera_control.OnUpdate(dt);

	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	
	Clove::Renderer2D::DrawQuad({0.0f,0.0f}, {0.5f,0.5f}, {0.9f, 0.0f, 0.5f, 1.0f});
	Clove::Renderer2D::DrawQuad({-1.0f,1.0f}, {2.0f,1.0f},{0.9f, 0.7f, 0.1f, 1.0f});
	Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f}, { 21.0f,21.0f }, m_TextureChess, 5.0f);

	Clove::QuadProperties props{};
	props.position = glm::vec3(2.0f, -1.0f, 0.0f);
	props.color = glm::vec4{ 0.0f,0.8f,0.5f,1.0f };
	props.texture = m_TextureChess;
	props.rotation = 45.0f;
	props.tiling_factor = 1.5f;
	Clove::Renderer2D::DrawQuad(props);
	Clove::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 0.5f,0.5f }, m_TextureRing);
	Clove::Renderer2D::EndScene();

	// This would be valid C++20, but switching version breaks other stuff:
	//Clove::Renderer2D::DrawQuad({ .color = glm::vec4{0.5f} });
	
	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	for (float x = -5.0f; x <= 5.0f; x += 0.5f) {
		for (float y = -5.0f; y <= 5.0f; y += 0.5f) {
			glm::vec4 color = {(x+5.0f)/10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f};
			Clove::Renderer2D::DrawQuad({ x,y,-0.05f}, { 0.45f,0.45f }, color);
		}
	}

	Clove::Renderer2D::EndScene();

	// particle system
	if (Clove::Input::IsMouseButtonPressed(Clove::MouseButton::MOUSE_BUTTON_LEFT)) {
		std::pair<float,float> mouse_pos = Clove::Input::GetMousePos();
		float x = mouse_pos.first, y = mouse_pos.second;

		float width = (float)Clove::GameApp::GetInstance().GetWindow().GetWidth();
		float height = (float)Clove::GameApp::GetInstance().GetWindow().GetHeight();

		const Clove::CameraBounds& bounds = m_camera_control.GetBounds();
		const glm::vec3& pos = m_camera_control.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - 0.5f * bounds.GetWidth();
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_particle.pos = { x + pos.x, y + pos.y };

		for (int i = 0; i < 5; ++i) m_particle_system.Emit(m_particle);
	}

	m_particle_system.OnUpdate(dt);

	m_particle_system.OnRender(m_camera_control.GetCamera());

}

void Game2D::OnImGuiRender() {

	Clove::Renderer2D::Statistics& stats = Clove::Renderer2D::GetStats();

	ImGui::Begin("Renderer Stats");
	ImGui::Text("FPS: %.1f", m_fps);
	ImGui::Text("Draw calls: %d", stats.draw_calls);
	ImGui::Text("Quads: %d", stats.quad_count);
	ImGui::Text("Vertices: %d", stats.GetVertexCount());
	ImGui::Text("Indices: %d", stats.GetIndexCount());
	ImGui::Text("Triangles: %d", stats.quad_count * 2);

	Clove::Renderer2D::ResetStats();

	ImGui::End();
}

void Game2D::OnEvent(Clove::Event& e) {
	m_camera_control.OnEvent(e);
}


