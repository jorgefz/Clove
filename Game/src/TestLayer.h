#pragma once

#include <Clove.h>


class TestLayer : public Clove::Layer {

public:
	TestLayer();
	void OnUpdate(float dt) override;
	void OnImGuiRender() override;
	void OnEvent(Clove::Event& e) override;
	bool OnKeyPressedEvent(Clove::KeyPressEvent& event);

private:
	Clove::ShaderLibrary m_shaderlib;
	Clove::Ref<Clove::VertexArray> m_vao;
	Clove::CameraController m_camera_control;

	Clove::Ref<Clove::Texture2D> m_texture, m_texture2;
};
