#include "ParticleSystem.h"
#include "glm/gtx/compatibility.hpp"

#include <random>

class Random {
public:
	static void Init() {
		s_random_engine.seed(std::random_device()());
	}

	static float Float() {
		float random_float = static_cast<float>(s_distribution(s_random_engine));
		return random_float / static_cast<float>(std::numeric_limits<uint32_t>::max());
	}

private:
	static std::mt19937 s_random_engine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};

std::mt19937 Random::s_random_engine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;




ParticleSystem::ParticleSystem(uint32_t max_particles) {
	m_pool_index = max_particles - 1;
	m_particle_pool.resize(max_particles);
	Random::Init();
}

void ParticleSystem::OnUpdate(float dt) {
	for (auto& particle : m_particle_pool) {
		if (particle.life_remaining <= 0.0f) particle.active = false;
		if (!particle.active) continue;

		particle.life_remaining -= dt;
		particle.pos += particle.vel * dt;
		particle.rotation += 0.01f * dt;
	}
}

void ParticleSystem::OnRender(Clove::Camera& camera, float zlevel) {

	Clove::Renderer2D::BeginScene(camera);


	for (auto& particle : m_particle_pool) {
		if (!particle.active) continue;
		
		// fade away
		float life_fade = particle.life_remaining / particle.lifetime;
		glm::vec4 color = glm::lerp(particle.color_end, particle.color_start, life_fade);
		float size = glm::lerp(particle.size_end, particle.size_start, life_fade);

		Clove::QuadProperties props{};
		props.position = { particle.pos.x, particle.pos.y, zlevel };
		props.rotation = particle.rotation;
		props.size = { size,size };
		props.color = color;

		Clove::Renderer2D::DrawQuad(props);
	}

	Clove::Renderer2D::EndScene();
}


void ParticleSystem::Emit(const ParticleProps& props) {

	Particle& particle = m_particle_pool[m_pool_index];

	particle.active = true;
	particle.pos = props.pos;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.vel = props.vel;
	particle.vel.x += props.acc.x * (Random::Float() - 0.5f);
	particle.vel.y += props.acc.y * (Random::Float() - 0.5f);

	particle.color_start = props.color_start;
	particle.color_end = props.color_start;
	
	particle.lifetime = props.lifetime;
	particle.life_remaining = props.lifetime;
	particle.size_start = props.size_start + props.size_change * (Random::Float() - 0.5f);
	particle.size_end = props.size_end + props.size_change * (Random::Float() - 0.5f);

	m_pool_index = --m_pool_index % m_particle_pool.size();
}
