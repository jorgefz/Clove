#include "clovepch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Clove/Renderer/Renderer2D.h"
#include "Clove/Scene/Entity.h"


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

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

}