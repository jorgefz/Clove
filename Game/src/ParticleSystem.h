#pragma once

#include <Clove.h>

struct ParticleProps {
	glm::vec2 pos, vel, acc;
	glm::vec4 color_start, color_end;
	float size_start, size_end, size_change;
	float lifetime = 1.0f;
};


class ParticleSystem {
public:
	ParticleSystem(uint32_t max_particles = 1000);
	void OnUpdate(float dt);
	void OnRender(Clove::Camera& camera, float zlevel = 1.0f);
	void Emit(const ParticleProps& props);

private:
	struct Particle {
		glm::vec2 pos, vel;
		glm::vec4 color_start, color_end;
		float rotation = 0.0f;
		float size_start, size_end;
		float lifetime = 1.0f;
		float life_remaining = 0.0f;
		bool active = false;
	};

	std::vector<Particle> m_particle_pool;
	uint32_t m_pool_index;
};

