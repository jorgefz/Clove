#pragma once

#include <Clove.h>


class Game2D : public Clove::Layer {
public:
	Game2D();
	~Game2D() = default;
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
	Clove::Ref<Clove::Texture2D> m_TextureChess;
	glm::vec4 m_color = { 0.2f, 0.3f, 0.8f, 1.0f };
};



