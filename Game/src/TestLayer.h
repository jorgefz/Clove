#pragma once

#include <Clove.h>


class TestLayer : public Clove::Layer {

public:
	TestLayer();
	~TestLayer() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnImGuiRender() override;
	void OnEvent(Clove::Event& e) override;
	bool OnKeyPressedEvent(Clove::KeyPressEvent& event);

private:
	Clove::ShaderLibrary m_shaderlib;
	Clove::CameraController m_camera_control;

	Clove::Ref<Clove::Texture2D> m_GrassTexture, m_RockTexture, m_PlayerTexture;
};
