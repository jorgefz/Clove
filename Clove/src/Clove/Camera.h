#pragma once
#include "clovepch.h"

#include "Core.h"
#include "Window.h"

namespace Clove {

	class Camera {
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::vec3 m_position;
		float m_rotation = 0.0f;
		float m_scale = 1.0f;
	public:
		Camera(float left, float right, float bottom, float top);
		~Camera();

		const glm::vec3& GetPosition() const;
		void SetPosition(glm::vec3& position);

		float GetRotation();
		void SetRotation(float rotation);

		float GetScale();
		void SetScale(float scale);

		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;
		void UpdateMatrices(Clove::Window& window);
	};
}

