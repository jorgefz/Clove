#include "clovepch.h"
#include "Camera.h"

namespace Clove {

	Camera::Camera(float left, float right, float bottom, float top) 
		: m_proj( glm::ortho(left, right, bottom, top, -1.0f, 1.0f) ),
		  m_view(glm::mat4(1.0f)),
		  m_view_proj( m_proj * m_view ),
		  m_position(glm::vec3(0.0f,0.0f,1.0f))
	{
	}

	Camera::~Camera() {  }

	const glm::vec3& Camera::GetPosition() const {
		return m_position;
	}
	void Camera::SetPosition(glm::vec3& position) {
		m_position = position;
	}

	float Camera::GetRotation() {
		return m_rotation;
	}
	void Camera::SetRotation(float rotation) {
		m_rotation = rotation;
	}

	float Camera::GetScale() {
		return m_scale;
	}
	void Camera::SetScale(float scale) {
		m_scale = scale;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const {
		return m_proj;
	}
	const glm::mat4& Camera::GetViewMatrix() const {
		return m_view;
	}
	const glm::mat4& Camera::GetViewProjectionMatrix() const {
		return m_view_proj;
	}

	void Camera::Update() {
		const glm::mat4 identity = glm::mat4(1.0f);
		glm::mat4 scaled = glm::scale(identity, glm::vec3(m_scale, m_scale, 1.0f)); // zoom
		glm::mat4 translated = glm::translate(identity, m_position);
		glm::mat4 rotated = glm::rotate(identity, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f)); //rotate about z axis
		glm::mat4 transform = translated * rotated * scaled;
		m_view = glm::inverse(transform); // memory-intensive operation

		// TODO: update projection on window resize
		m_view_proj = m_proj * m_view;
	}

}