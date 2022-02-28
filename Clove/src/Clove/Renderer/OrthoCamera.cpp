#include "clovepch.h"
#include "OrthoCamera.h"

namespace Clove {

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
		: m_proj( glm::ortho(left, right, bottom, top, -1.0f, 1.0f) ),
		  m_view(1.0f),
		  m_position(0.0f,0.0f,0.0f)
	{
		CLOVE_PROFILE_FUNCTION();
		m_view_proj = m_proj * m_view;
	}

	OrthoCamera::OrthoCamera(const glm::mat4& projection)
		: m_proj(projection), m_view(1.0f), m_position(0.0f)
	{
		CLOVE_PROFILE_FUNCTION();
		m_view_proj = m_proj * m_view;
	}

	OrthoCamera::~OrthoCamera() {  }

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top) {
		CLOVE_PROFILE_FUNCTION();
		m_proj = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_view_proj = m_proj * m_view;
	}

	const glm::vec3& OrthoCamera::GetPosition() const {
		return m_position;
	}
	void OrthoCamera::SetPosition(glm::vec3& position) {
		m_position = position;
	}

	float OrthoCamera::GetRotation() {
		return m_rotation;
	}
	void OrthoCamera::SetRotation(float rotation) {
		m_rotation = rotation;
	}

	float OrthoCamera::GetScale() {
		return m_scale;
	}
	void OrthoCamera::SetScale(float scale) {
		m_scale = scale;
	}

	const glm::mat4& OrthoCamera::GetProjectionMatrix() const {
		return m_proj;
	}
	const glm::mat4& OrthoCamera::GetViewMatrix() const {
		return m_view;
	}
	const glm::mat4& OrthoCamera::GetViewProjectionMatrix() const {
		return m_view_proj;
	}

	void OrthoCamera::Update() {
		CLOVE_PROFILE_FUNCTION();
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