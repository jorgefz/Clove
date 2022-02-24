#include "clovepch.h"
#include "Scene.h"

#include "Clove/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Clove {



	Scene::Scene() {

	}

	Scene::~Scene() {
		

	}

	void Scene::OnUpdate(float dt) {
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.color);
		}
	}

	entt::entity Scene::CreateEntity() {
		return m_registry.create();
	}

}