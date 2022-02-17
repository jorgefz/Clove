#pragma once

#include <Clove.h>

namespace Clove {

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		~EditorLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float dt) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		void EditorLayer::DockSpace();

	private:
		CameraController m_camera_control;
		Ref<Framebuffer> m_framebuffer;

		// These will be abstracted away by 2D renderer
		Ref<Shader> m_shader;
		Ref<VertexArray> m_vao;
		Ref<Texture2D> m_TextureChess;
		Ref<Texture2D> m_TextureRing;
		Ref<Texture2D> m_TilesetNature;
		Ref<SubTexture2D> m_TileGrass, m_TileBush, m_TileSign, m_TileTree;
		std::unordered_map<char, Ref<SubTexture2D>> m_TextureMap;

		bool m_mouse_dragging = false;
		glm::vec2 m_mouse_pos_prev = { 0.0f,0.0f };
		glm::vec4 m_color = { 0.2f, 0.3f, 0.8f, 1.0f };
		float m_fps = 0.0f;

		glm::vec2 m_viewport_size = {0.0f, 0.0f};
	};

}



