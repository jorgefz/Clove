#pragma once

#include <Clove.h>

#include "ParticleSystem.h"


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
	Clove::Ref<Clove::Texture2D> m_TextureRing;
	Clove::Ref<Clove::Texture2D> m_TilesetNature;
	Clove::Ref<Clove::SubTexture2D> m_TileGrass, m_TileBush, m_TileSign, m_TileTree;

	std::unordered_map<char, Clove::Ref<Clove::SubTexture2D>> m_TextureMap;

	ParticleProps m_particle;
	ParticleSystem m_particle_system;

	bool m_mouse_dragging = false;
	glm::vec2 m_mouse_pos_prev = {0.0f,0.0f};

	glm::vec4 m_color = { 0.2f, 0.3f, 0.8f, 1.0f };
	float m_fps = 0.0f;
};



