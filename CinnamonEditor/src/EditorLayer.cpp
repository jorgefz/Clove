

#include "EditorLayer.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"


enum Tiles {
	TILE_GRASS = '0',
	TILE_WATER = '1',
	TILE_DIRT  = '2',
	TILE_STONE = '3'
};


const glm::vec2 MapSize = {24, 12};

static const char* s_MapTiles = 
"333333333333300000000000"
"333333333330000000000000"
"330003333300000000002222"
"300000330000000022222200"
"000000000022222220000000"
"000000002222000000000000"
"000000022000000000000000"
"002222222000000000011111"
"022000000000001111111111"
"220000000001111111111111"
"000000000001111111111111"
"000000000111111111111111";


namespace Clove {

	static float RandomFloat(float lower = 0.0f, float upper = 1.0f) {
		return lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upper - lower)));
	}

	static glm::vec2 ScreenToWorld(const glm::vec2& pos, const CameraController& camera_control) {

		float width = (float)GameApp::GetInstance().GetWindow().GetWidth();
		float height = (float)GameApp::GetInstance().GetWindow().GetHeight();

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

		m_TileGrass = SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,6 }, { 32,32 });

		m_TileBush = SubTexture2D::CreateFromCoords(m_TilesetNature, { 1,7 }, { 32,32 });
		m_TileSign = SubTexture2D::CreateFromCoords(m_TilesetNature, { 2,8 }, { 32,32 });
		m_TileTree = SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,7 }, { 32,32 }, { 1,2 });

		m_TextureMap[TILE_GRASS] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,6 }, { 32,32 });
		m_TextureMap[TILE_WATER] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 1,6 }, { 32,32 });
		m_TextureMap[TILE_DIRT] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 2,6 }, { 32,32 });
		m_TextureMap[TILE_STONE] = SubTexture2D::CreateFromCoords(m_TilesetNature, { 3,6 }, { 32,32 });


		FramebufferSpec fbspec{};
		fbspec.width = GameApp::Get().GetWindow().GetWidth();
		fbspec.height = GameApp::Get().GetWindow().GetHeight();
		m_framebuffer = Framebuffer::Create(fbspec);
	}


	void EditorLayer::OnDetach() {
		CLOVE_PROFILE_FUNCTION();
		Renderer2D::Shutdown();
	}

	void EditorLayer::OnUpdate(float dt) {

		m_fps = 1.0f / dt;
		m_camera_control.OnUpdate(dt);
		Renderer2D::ResetStats();

		if (m_enable_dockspace) m_framebuffer->Bind();

		RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_camera_control.GetCamera());

		QuadProperties tilemap{};
		for (uint32_t y = 0; y != (uint32_t)MapSize.y; y++) {
			for (uint32_t x = 0; x != (uint32_t)MapSize.x; x++) {
				char key = s_MapTiles[x + y * (uint32_t)MapSize.x];
				tilemap.subtexture = m_TextureMap[key];
				tilemap.position = { (float)(y)-MapSize.x / 2.0f, (float)(x)-MapSize.y / 2.0f, 0.0f };
				Renderer2D::DrawQuad(tilemap);
			}
		}

		QuadProperties tiles{};
		tiles.subtexture = m_TileBush;
		tiles.position = { -1.0f, 0.0f, 0.1f };
		Renderer2D::DrawQuad(tiles);

		tiles.subtexture = m_TileSign;
		tiles.position = { 0.0f, 1.0f, 0.1f };
		Renderer2D::DrawQuad(tiles);

		tiles.size = { 1,2 };
		for (float x = -2.0f; x <= 2.0f; x += 1.0f) {
			tiles.subtexture = m_TileTree;
			tiles.position = { x, 3.0f, 0.1f };
			Renderer2D::DrawQuad(tiles);
		}
		Renderer2D::EndScene();
		if(m_enable_dockspace) m_framebuffer->Unbind();

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

		if (m_enable_dockspace) {
			EditorLayer::TestDockSpace();
		}
		else {
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
		}

	}



	void EditorLayer::TestDockSpace() {
		// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
		// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
		// In this specific demo, we are not using DockSpaceOverViewport() because:
		// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
		// - we allow the host window to have padding (when opt_padding == true)
		// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
		// TL;DR; this demo is more complicated than what you would normally use.
		// If we removed all the options we are showcasing, this demo would become:
		//     void ShowExampleAppDockSpace()
		//     {
		//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//     }

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

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &DockspaceOpen, window_flags);

		if (!opt_padding) ImGui::PopStyleVar();
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

		uint32_t id = m_framebuffer->GetColorAttachmentID();
		ImGui::Image((void*)id, ImVec2{ 1280.0f, 720.0f }, { 0,1 }, { 1,0 }); // last two arguments flips the texture upright
		ImGui::End();

		ImGui::End();
	}

}