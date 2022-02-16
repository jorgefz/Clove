

#include "Game2D.h"

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




static float RandomFloat(float lower = 0.0f, float upper = 1.0f) {
	return lower + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upper - lower)));
}

static glm::vec2 ScreenToWorld(const glm::vec2& pos, const Clove::CameraController& camera_control) {
	
	float width = (float)Clove::GameApp::GetInstance().GetWindow().GetWidth();
	float height = (float)Clove::GameApp::GetInstance().GetWindow().GetHeight();

	const Clove::CameraBounds& bounds = camera_control.GetBounds();

	float x = (pos.x / width) * bounds.GetWidth() - 0.5f * bounds.GetWidth();
	float y = bounds.GetHeight() * 0.5f - (pos.y / height) * bounds.GetHeight();
	
	return glm::vec2{ x, y };
}


Game2D::Game2D()
	: Layer("Game2D"), m_camera_control(16.0f/9.0f)
{

}


void Game2D::OnAttach() {
	CLOVE_PROFILE_FUNCTION();
	m_TextureChess = Clove::Texture2D::Create("assets/checkerboard.png");
	m_TextureRing = Clove::Texture2D::Create("assets/cursor.png");
	m_TilesetNature = Clove::Texture2D::Create("assets/tilesets/nature.png");
	
	m_TileGrass = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, {0,6}, {32,32});

	m_TileBush = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, { 1,7 }, {32,32});
	m_TileSign = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, { 2,8 }, {32,32});
	m_TileTree = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, { 0,7 }, {32,32}, {1,2});

	m_TextureMap[TILE_GRASS] = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, {0,6}, {32,32});
	m_TextureMap[TILE_WATER] = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, {1,6}, {32,32});
	m_TextureMap[TILE_DIRT]  = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, {2,6}, {32,32});
	m_TextureMap[TILE_STONE] = Clove::SubTexture2D::CreateFromCoords(m_TilesetNature, {3,6}, {32,32});


	// particle system
	m_particle.color_start = { 254.0f/255.0f, 212.0f/255.0f, 123.0f/255.0f, 1.0f };
	m_particle.color_start = { 254.0f/255.0f, 109.0f/255.0f, 41.0f/255.0f, 1.0f };
	m_particle.size_start = 0.5f, m_particle.size_change = 0.3f, m_particle.size_end = 0.0f;
	m_particle.lifetime = 1.0f;
	m_particle.vel = { 0.0f,0.0f }, m_particle.acc = { 3.0f,1.0f }, m_particle.pos = { 0.0f,0.0f };
}


void Game2D::OnDetach() {
	CLOVE_PROFILE_FUNCTION();
	Clove::Renderer2D::Shutdown();
}

void Game2D::OnUpdate(float dt) {

	m_fps = 1.0f / dt;

	m_camera_control.OnUpdate(dt);

	Clove::RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	Clove::RenderCommand::Clear();

	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	
#if 0
	Clove::Renderer2D::DrawQuad({0.0f,0.0f}, {0.5f,0.5f}, {0.9f, 0.0f, 0.5f, 1.0f});
	Clove::Renderer2D::DrawQuad({-1.0f,1.0f}, {2.0f,1.0f},{0.9f, 0.7f, 0.1f, 1.0f});
	Clove::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f}, { 21.0f,21.0f }, m_TextureChess, 5.0f);

	Clove::QuadProperties props{};
	props.position = glm::vec3(2.0f, -1.0f, 0.0f);
	props.color = glm::vec4{ 0.0f,0.8f,0.5f,1.0f };
	props.texture = m_TextureChess;
	props.rotation = 45.0f;
	props.tiling_factor = 1.5f;
	Clove::Renderer2D::DrawQuad(props);
	Clove::Renderer2D::DrawQuad({ 1.0f,0.0f }, { 0.5f,0.5f }, m_TextureRing);
	Clove::Renderer2D::EndScene();

	// This would be valid C++20, but switching version breaks other stuff:
	//Clove::Renderer2D::DrawQuad({ .color = glm::vec4{0.5f} });
	
	Clove::Renderer2D::BeginScene(m_camera_control.GetCamera());
	for (float x = -5.0f; x <= 5.0f; x += 0.5f) {
		for (float y = -5.0f; y <= 5.0f; y += 0.5f) {
			glm::vec4 color = {(x+5.0f)/10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f};
			Clove::Renderer2D::DrawQuad({ x,y,-0.05f}, { 0.45f,0.45f }, color);
		}
	}
#endif

	Clove::QuadProperties tilemap{};

	for (uint32_t y = 0; y != (uint32_t)MapSize.y; y++) {
		for (uint32_t x = 0; x != (uint32_t)MapSize.x; x++) {
			char key = s_MapTiles[x + y * (uint32_t)MapSize.x];
			tilemap.subtexture = m_TextureMap[key];
			tilemap.position = { (float)(y)-MapSize.x/2.0f, (float)(x)-MapSize.y/2.0f, 0.0f };
			Clove::Renderer2D::DrawQuad(tilemap);
		}
	}

	Clove::QuadProperties tiles{};
	tiles.subtexture = m_TileBush;
	tiles.position = { -1.0f, 0.0f, 0.1f };
	Clove::Renderer2D::DrawQuad(tiles);

	tiles.subtexture = m_TileSign;
	tiles.position = { 0.0f, 1.0f, 0.1f };
	Clove::Renderer2D::DrawQuad(tiles);

	tiles.size = { 1,2 };
	for (float x = -2.0f; x <= 2.0f; x += 1.0f) {
		tiles.subtexture = m_TileTree;
		tiles.position = { x, 3.0f, 0.1f };
		Clove::Renderer2D::DrawQuad(tiles);
	}


	Clove::Renderer2D::EndScene();

	// Generates particles for particle system
	if (Clove::Input::IsMouseButtonPressed(Clove::MouseButton::MOUSE_BUTTON_RIGHT)) {

		std::pair<float, float> mouse_pos = Clove::Input::GetMousePos();
		glm::vec2 ds = ScreenToWorld({mouse_pos.first, mouse_pos.second}, m_camera_control);
		const glm::vec3& pos = m_camera_control.GetCamera().GetPosition();
		m_particle.pos = { ds.x + pos.x, ds.y + pos.y };

		for (int i = 0; i < 10; ++i) m_particle_system.Emit(m_particle);
	}
	m_particle_system.OnUpdate(dt);
	m_particle_system.OnRender(m_camera_control.GetCamera());
}

void Game2D::OnImGuiRender() {

	Clove::Renderer2D::Statistics& stats = Clove::Renderer2D::GetStats();

	ImGui::Begin("Renderer Stats");
	ImGui::Text("FPS: %.1f", m_fps);
	ImGui::Text("Draw calls: %d", stats.draw_calls);
	ImGui::Text("Quads: %d", stats.quad_count);
	ImGui::Text("Vertices: %d", stats.GetVertexCount());
	ImGui::Text("Indices: %d", stats.GetIndexCount());
	ImGui::Text("Triangles: %d", stats.quad_count * 2);

	Clove::Renderer2D::ResetStats();

	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}


void Game2D::OnEvent(Clove::Event& e) {
	m_camera_control.OnEvent(e);


	// These events enable mouse dragging to move camera
	Clove::EventDispatcher dp(e);
	dp.Dispatch<Clove::MouseButtonPressedEvent>([&, this](Clove::MouseButtonPressedEvent& e) {
		if (e.GetMouseButton() == Clove::MouseButton::MOUSE_BUTTON_LEFT) {
			m_mouse_dragging = true;
		}
		return false;
	});

	dp.Dispatch<Clove::MouseButtonReleasedEvent>([&, this](Clove::MouseButtonReleasedEvent& e) {
		if (e.GetMouseButton() == Clove::MouseButton::MOUSE_BUTTON_LEFT) {
			m_mouse_dragging = false;
		}
		return false;
	});

	dp.Dispatch<Clove::MouseMovedEvent>([&, this](Clove::MouseMovedEvent& e) {
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


