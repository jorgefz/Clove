#pragma once
#include "entt.hpp"
#include "Clove/Scene/Components.h"

namespace Clove {

	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();
		void OnUpdate(float dt);

		entt::registry& Reg() { return m_registry; };

	private:
		entt::registry m_registry;

		friend class Entity;
	};
}
