#pragma once

#include <Clove.h>

struct GameObject {
	glm::vec2 pos, vel, scale;
	Clove::Key up, down;
	bool colliding = false, moving = false;
};


class Pong : public Clove::Layer {
public:
	Pong();
	~Pong() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnImGuiRender() override;
	void OnEvent(Clove::Event& e) override;

private:

	Clove::CameraController m_camera_control;

	// These will be abstracted away by 2D renderer
	Clove::Ref<Clove::Shader> m_shader;
	Clove::Ref<Clove::VertexArray> m_vao;

	Clove::Ref<Clove::Texture2D> m_TextureBall;

	GameObject m_ball, m_lpaddle, m_rpaddle;
	std::pair<int,int> scores = {0,0};

	glm::vec4 m_paddle_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};

