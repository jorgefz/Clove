#pragma once
#include "Clove/Core/Core.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Clove/Core/Window.h"

/*

Describes how the world is viewed and translated to the screen view.
Sets up a Model View Projection matrix which is then passed to the shader
to transform each vertex.

*/

namespace Clove {

	class Camera {
	public:
		Camera(float left, float right, float bottom, float top);
		~Camera();

		void SetProjection(float left, float right, float bottom, float top);
		void SetPosition(glm::vec3& position);
		const glm::vec3& GetPosition() const;

		void SetRotation(float rotation);
		float GetRotation();

		void SetScale(float scale);
		float GetScale();

		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjectionMatrix() const;
		void Update();
	
	private:
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_view_proj;
		glm::vec3 m_position;
		float m_rotation = 0.0f;
		float m_scale = 1.0f;
	};
}

