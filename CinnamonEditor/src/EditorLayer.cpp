

#include "EditorLayer.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"


enum Tiles {
	TILE_GRASS = '0',
	TILE_WATER = '1',
	TILE_DIRT  = '2',
	TILE_STONE = '3',
	TILE_TREE =  'T',
	TILE_BUSH =  'B',
	TILE_SIGN =  'S'
};


const glm::vec2 MapSize = {24, 12};

static const char* s_MapTiles = 
"333333333333300000000000"
"333333333330000T0000S000"
"33000333330T000000002222"
"300000330000B00022222200"
"0000000B0022222220000000"
"0000B000222200000T000000"
"0000S00220000T0000000000"
"00222222200T000000011111"
"02200BB00000001111111111"
"2200B0000001111111111111"
"000000TT0001111111111111"
"00B000000111111111111111";


namespace Clove {

	static float RandomFloat(float lower = 0.0f, float upper = 1.0f) {
		return lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upper - lower)));
	}

	static glm::vec2 ScreenToWorld(const glm::vec2& pos, const CameraController& camera_control) {

		float width = (float)GameApp::Get().GetWindow().GetWidth();
		float height = (float)GameApp::Get().GetWindow().GetHeight();

		const CameraBounds& bounds = camera_control.GetBounds();

		float x = (pos.x / width) * bounds.GetWidth() - 0.5f * bounds.GetWidth();
		float y = bounds.GetHeight() * 0.5f - (pos.y / height) * bounds.GetHeight();

		return glm::vec2{ x, y };
	}




	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_camera_control(16.0f / 9.0f)
	{

	}


	void EditorLayer::OnAttach() {
		CLOVE_PROFILE_FUNCTION();
		m_TextureChess = Texture2D::Create("assets/checkerboard.png");
		m_TextureRing = Texture2D::Create("assets/cursor.png");
		m_TilesetNature = Texture2D::Create("assets/tilesets/nature.png");
		
		m_TextureMap[TILE_GRASS] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,6 }, { 32,32 });
		m_TextureMap[TILE_WATER] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 1,6 }, { 32,32 });
		m_TextureMap[TILE_DIRT] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 2,6 }, { 32,32 });
		m_TextureMap[TILE_STONE] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 3,6 }, { 32,32 });
		m_TextureMap[TILE_BUSH] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 1,7 }, { 32,32 });
		m_TextureMap[TILE_SIGN] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 2,8 }, { 32,32 });
		m_TextureMap[TILE_TREE] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,7 }, { 32,32 }, { 1,2 });

		FramebufferSpec fbspec{};
		fbspec.width = GameApp::Get().GetWindow().GetWidth();
		fbspec.height = GameApp::Get().GetWindow().GetHeight();
		m_framebuffer = Framebuffer::Create(fbspec);

		m_active_scene = CreateRef<Scene>();
		
		auto square = m_active_scene->CreateEntity();
		Entity square_entity = { square, m_active_scene.get() };
		square_entity.HasComponent<TransformComponent>();

		m_active_scene->Reg().emplace<TransformComponent>(square);
		m_active_scene->Reg().emplace<SpriteRendererComponent>(square, glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});
	}


	void EditorLayer::OnDetach() {
		CLOVE_PROFILE_FUNCTION();
		Renderer2D::Shutdown();
	}

	void EditorLayer::OnUpdate(float dt) {

		// this disables camera movement when the main scene is not selected
		if (m_viewport_focused) {
			m_camera_control.OnUpdate(dt);
		}
		m_fps = 1.0f / dt;
		
		Renderer2D::ResetStats();
		m_framebuffer->Bind();
		RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		RenderCommand::Clear();


		Renderer2D::BeginScene(m_camera_control.GetCamera());

		// update scene
		m_active_scene->OnUpdate(dt);


		for (uint32_t y = 0; y != (uint32_t)MapSize.y; y++) {
			for (uint32_t x = 0; x != (uint32_t)MapSize.x; x++) {
				
				QuadProperties tilemap{};
				char key = s_MapTiles[x + y * (uint32_t)MapSize.x];
				tilemap.position = { (float)(y)-MapSize.x / 2.0f, (float)(x)-MapSize.y / 2.0f, 0.0f };
				
				// draw grass below tiles with transparency
				if (key == TILE_BUSH || key == TILE_SIGN) {
					tilemap.subtexture = m_TextureMap[TILE_GRASS];
					Renderer2D::DrawQuad(tilemap);
					tilemap.position.z += 0.1f;
				}
				else if (key == TILE_TREE ){
					tilemap.subtexture = m_TextureMap[TILE_GRASS];
					tilemap.size = { 1,2 };
					Renderer2D::DrawQuad(tilemap);
					tilemap.position.z += 0.1f;
				}
				
				tilemap.subtexture = m_TextureMap[key];
				Renderer2D::DrawQuad(tilemap);
			}
		}

		Renderer2D::EndScene();
		m_framebuffer->Unbind();

	}



	void EditorLayer::OnEvent(Event& e) {
		m_camera_control.OnEvent(e);


		// These events enable mouse dragging to move camera
		EventDispatcher dp(e);
		dp.Dispatch<MouseButtonPressedEvent>([&, this](MouseButtonPressedEvent& e) {
			if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT) {
				m_mouse_dragging = true;
			}
			return false;
		});

		dp.Dispatch<MouseButtonReleasedEvent>([&, this](MouseButtonReleasedEvent& e) {
			if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT) {
				m_mouse_dragging = false;
			}
			return false;
		});

		dp.Dispatch<MouseMovedEvent>([&, this](MouseMovedEvent& e) {
			if (!m_mouse_dragging) {
				m_mouse_pos_prev = { e.GetX(), e.GetY() };
				return false;
			}

			glm::vec2 old_pos = ScreenToWorld(m_mouse_pos_prev, m_camera_control);
			glm::vec2 new_pos = ScreenToWorld({ e.GetX(), e.GetY() }, m_camera_control);
			glm::vec2 move = old_pos - new_pos;

			glm::vec3 cam_pos = m_camera_control.GetCamera().GetPosition();
			m_camera_control.GetCamera().SetPosition(glm::vec3{ cam_pos.x + move.x, cam_pos.y + move.y, cam_pos.z });

			m_mouse_pos_prev = { e.GetX(), e.GetY() };
			return false;
		});
	}





	void EditorLayer::OnImGuiRender() {
		EditorLayer::DockSpace();

	}



	void EditorLayer::DockSpace() {

		static bool DockspaceOpen = true;

		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::Begin("DockSpace", &DockspaceOpen, window_flags);

		if (opt_fullscreen) ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit")) GameApp::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		Renderer2D::Statistics& stats = Renderer2D::GetStats();

		ImGui::Begin("Renderer Stats");
		ImGui::Text("FPS: %.1f", m_fps);
		ImGui::Text("Draw calls: %d", stats.draw_calls);
		ImGui::Text("Quads: %d", stats.quad_count);
		ImGui::Text("Vertices: %d", stats.GetVertexCount());
		ImGui::Text("Indices: %d", stats.GetIndexCount());
		ImGui::Text("Triangles: %d", stats.quad_count * 2);
		Renderer2D::ResetStats();
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		m_viewport_focused = ImGui::IsWindowFocused();
		m_viewport_hovered = ImGui::IsWindowHovered();
		GameApp::Get().GetImGuiLayer()->BlockEvents(!m_viewport_focused || !m_viewport_hovered);

		ImVec2 im_vp_size = ImGui::GetContentRegionAvail();
		glm::vec2 vp_size = glm::vec2({ im_vp_size.x, im_vp_size.y });

		if (m_viewport_size != vp_size) {
			m_viewport_size = vp_size;
			m_framebuffer->Resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
			m_camera_control.ResizeView(m_viewport_size.x, m_viewport_size.y);
		}
		
		uint32_t id = m_framebuffer->GetColorAttachmentID();
		// the last two arguments flips the texture upright:
		ImGui::Image((void*)(uintptr_t)id, im_vp_size, { 0,1 }, { 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}

}



