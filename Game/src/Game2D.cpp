

#include "Game2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <chrono>

template<typename Fn>
class Timer {
public:
	Timer(const char* name, Fn&& func)
		: m_name(name), m_stopped(false), m_func(func)
	{
		m_start_point = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!m_stopped) Timer::Stop();
	}

	void Stop() {
		auto end_point = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_point).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
		m_stopped = true;
		float duration = static_cast<float>(end - start) * 0.001f;
		m_func({ m_name, duration });
	}

private:
	const char* m_name;
	bool m_stopped;
	std::chrono::time_point<std::chrono::steady_clock> m_start_point;
	Fn m_func;
};


#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfilerResult result) { m_profilers.push_back(result); })

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

	PROFILE_SCOPE("Game2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_camera_control.OnUpdate(dt);
	}

	{
		PROFILE_SCOPE("Render Prep");
		Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Clove::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Render Scene");
		Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
		Clove::Renderer2D::DrawQuad({0.0f,0.0f}, {0.5f,0.5f}, {0.9f, 0.0f, 0.5f, 1.0f});
		Clove::Renderer2D::DrawQuad({-1.0f,1.0f}, {2.0f,1.0f}, {0.9f, 0.7f, 0.1f, 1.0f});
		Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f}, { 21.0f,21.0f }, m_TextureChess);
		Clove::Renderer2D::EndScene();
	}
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


