

#include "Pong.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"



float RandomFloat(float min, float max) {
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

bool RandomBool() {
	return (rand() > RAND_MAX / 2);
}

bool IsPaddleMovingUp(GameObject& paddle) {
	return (Clove::Input::IsKeyPressed(paddle.up));
}

bool IsPaddleMovingDown(GameObject& paddle) {
	return (Clove::Input::IsKeyPressed(paddle.down));
}

void MovePaddle(GameObject& paddle, float dt, float top, float bottom) {
	paddle.moving = false;
	if (Clove::Input::IsKeyPressed(paddle.up)) {
		paddle.moving = true;
		if (!(paddle.pos.y + paddle.scale.y / 2.0f >= top))
			paddle.pos.y += paddle.vel.y * dt;
	}
	if (Clove::Input::IsKeyPressed(paddle.down)) {
		paddle.moving = true;
		if (!(paddle.pos.y - paddle.scale.y / 2.0f <= bottom))
			paddle.pos.y -= paddle.vel.y * dt;
	}
}

void CollidePaddle(GameObject& ball, GameObject& paddle) {

	// same y-axis range
	if (ball.pos.y <= paddle.pos.y + paddle.scale.y / 2.0f &&
		ball.pos.y >= paddle.pos.y - paddle.scale.y / 2.0f) {

		if (ball.pos.x - ball.scale.x / 2.0f <= paddle.pos.x + paddle.scale.x / 2.0f &&
			ball.pos.x + ball.scale.x / 2.0f >= paddle.pos.x - paddle.scale.x / 2.0f)
		{
			if (IsPaddleMovingUp(paddle)) ball.vel.y *= (ball.vel.y < 0.0f ? 0.8f : 1.2f);
			else if (IsPaddleMovingDown(paddle)) ball.vel.y *= (ball.vel.y > 0.0f ? 0.8f : 1.2f);
			ball.vel.x *= -1.0f;
			ball.collisions += 1;
		}
	}
}





Pong::Pong()
	: Layer("Pong"), m_camera_control(16.0f / 9.0f)
{  }


void Pong::OnAttach() {

	srand(time(NULL));

	m_TextureBall = Clove::Texture2D::Create("assets/cursor.png");

	m_ball.pos = glm::vec2(0.0f);
	m_ball.vel = glm::vec2(0.0f);
	m_ball.scale = glm::vec2(0.1f, 0.1f);

	m_lpaddle.pos = glm::vec2(-1.0f, 0.0f);
	m_lpaddle.vel = glm::vec2(0.0f, 1.7f);
	m_lpaddle.scale = glm::vec2(0.1f, 0.5f);
	m_lpaddle.up = Clove::Key::KEY_W;
	m_lpaddle.down = Clove::Key::KEY_S;

	m_rpaddle.pos = glm::vec2(1.0f, 0.0f);
	m_rpaddle.vel = glm::vec2(0.0f, 1.7f);
	m_rpaddle.scale = glm::vec2(0.1f, 0.5f);
	m_rpaddle.up = Clove::Key::KEY_O;
	m_rpaddle.down = Clove::Key::KEY_L;

}


void Pong::OnDetach() {
	Clove::Renderer2D::Shutdown();
}


void Pong::OnUpdate(float dt) {

	// pass 'dt' as argument to allow camera movement
	m_camera_control.OnUpdate(0);

	float wall_top = 1.0f;
	float wall_bottom = -1.0f;
	float wall_right = 1.25f;
	float wall_left = -1.25f;

	// Paddle movement
	MovePaddle(m_lpaddle, dt, wall_top, wall_bottom);
	MovePaddle(m_rpaddle, dt, wall_top, wall_bottom);

	// Ball movement
	if (Clove::Input::IsKeyPressed(Clove::Key::KEY_SPACE) && m_ball.moving == false) {
		m_ball.vel.x = RandomFloat(0.9f, 1.2f) * pow(-1.0f, (float)RandomBool());
		m_ball.vel.y = RandomFloat(0.2f, 0.7f) * pow(-1.0f, (float)RandomBool());
		m_ball.moving = true;
	}
	m_ball.pos.x += m_ball.vel.x * dt;
	m_ball.pos.y += m_ball.vel.y * dt;

	// Collision with tom/bottom walls
	if (m_ball.pos.y + m_ball.scale.y/2.0f >= wall_top ) {
		m_ball.vel.y *= -1.0f;
		m_ball.pos.y = wall_top * 0.95f;
	}
	else if (m_ball.pos.y - m_ball.scale.y / 2.0f <= wall_bottom) {
		m_ball.vel.y *= -1.0f;
		m_ball.pos.y = wall_bottom * 0.95f;
	}

	// Player 1 scoring
	if (m_ball.pos.x - m_ball.scale.y/2.0f <= m_lpaddle.pos.x) {
		m_ball.pos = glm::vec2(0.0f, 0.0f);
		m_ball.vel = glm::vec2(0.0f, 0.0f);
		m_ball.moving = false;
		scores.first += 1;
	}
	// Player 2 scoring
	else if (m_ball.pos.x + m_ball.scale.y / 2.0f >= m_rpaddle.pos.x) {
		m_ball.pos = glm::vec2(0.0f, 0.0f);
		m_ball.vel = glm::vec2(0.0f, 0.0f);
		m_ball.moving = false;
		scores.second += 1;
	}

	// Collision with paddles
	CollidePaddle(m_ball, m_lpaddle);
	CollidePaddle(m_ball, m_rpaddle);
	

	// Rendering
	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());

	// draw frame (game area)
	Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.2f }, { 2.5f,2.0f }, { 1.0f,1.0f,1.0f,1.0f }); //outer
	Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 2.45f,1.95f }, { 0.2f,0.2f,0.2f,1.0f }); //inner

	// draw paddles
	Clove::Renderer2D::DrawQuad(m_lpaddle.pos, m_lpaddle.scale, m_paddle_color);
	Clove::Renderer2D::DrawQuad(m_rpaddle.pos, m_rpaddle.scale, m_paddle_color);
	
	// draw ball
	Clove::Renderer2D::DrawQuad(m_ball.pos, m_ball.scale, m_TextureBall);
	
	Clove::Renderer2D::EndScene();
}

void Pong::OnImGuiRender() {
	
	ImGui::Begin("Debug");
	ImGui::Text("Player 1 Score: %d", scores.first);
	ImGui::Text("Player 2 Score: %d", scores.second);
	ImGui::Text("Ball speed: %.2f %.2f", m_ball.vel.x, m_ball.vel.y);
	ImGui::Text("Ball collisions: %d ", m_ball.collisions);
	ImGui::End();
}

void Pong::OnEvent(Clove::Event& e) {
	//m_camera_control.OnEvent(e);
}
