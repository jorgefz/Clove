#pragma once
#include "entt.hpp"
#include "Clove/Scene/Scene.h"

namespace Clove {

	class Entity {
	public:
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() {
			m_scene->m_registry.has<T>(m_handle);
		}
	
	private:
		entt::entity m_handle;
		Scene* m_scene;
	};

}

