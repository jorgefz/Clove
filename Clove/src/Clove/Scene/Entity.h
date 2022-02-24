#pragma once
#include "entt.hpp"
#include "Clove/Scene/Scene.h"

namespace Clove {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		
		template<typename T>
		bool HasComponent() {
			return m_scene->m_registry.any_of<T>(m_handle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			CLOVE_ASSERT(!HasComponent<T>(), "Entity already has this component.");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			CLOVE_ASSERT(HasComponent<T>(), "Entity does not have this component.");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		void RemoveComponent() {
			CLOVE_ASSERT(HasComponent<T>(), "Entity does not have this component.");
			m_scene->m_registry.remove<T>(m_handle);
		}

		operator bool() const { return (int)m_handle != entt::null; }
	
	private:
		entt::entity m_handle{ 0 };
		Scene* m_scene;
	};

}

