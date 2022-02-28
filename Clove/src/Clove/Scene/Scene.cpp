#include "clovepch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Clove/Renderer/Renderer2D.h"
#include "Clove/Scene/Entity.h"
#include "Clove/Renderer/OrthoCamera.h"


namespace Clove {



	Scene::Scene() {

	}

	Scene::~Scene() {
		

	}

	void Scene::OnUpdate(float dt) {
		
		Camera* main_camera = nullptr;
		glm::mat4* camera_transform = nullptr;
		{
			auto camera_group = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : camera_group) {
				auto& [transform, camera] = camera_group.get<TransformComponent, CameraComponent>(entity);
				if (!camera.primary) continue;
				main_camera = &camera.camera;
				camera_transform = &transform.transform;
				break;
			}
		}

		if (!main_camera) return; // can't render without a main camera
		
		Renderer2D::BeginScene(*main_camera, *camera_transform);

		auto sprite_group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : sprite_group) {
			auto& [transform, sprite] = sprite_group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.color);
		}

		Renderer2D::EndScene();
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity = { m_registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

}