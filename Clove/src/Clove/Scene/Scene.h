#pragma once
#include "entt.hpp"
#include "Clove/Scene/Components.h"

namespace Clove {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(float dt);

	private:
		entt::registry m_registry;

		friend class Entity;
	};
}
