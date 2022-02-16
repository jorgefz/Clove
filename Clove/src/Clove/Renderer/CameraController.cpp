#include <clovepch.h>
#include "Clove/Renderer/CameraController.h"
#include "Clove/Core/Input.h"
#include "Clove/Core/KeyCodes.h"

namespace Clove {
	CameraController::CameraController(float aspect_ratio, bool rotation)
		: m_rotation(rotation), m_aspect_ratio(aspect_ratio),
		  m_bounds({ -aspect_ratio * m_zoom_level, aspect_ratio* m_zoom_level, -m_zoom_level, m_zoom_level }),
		  m_cam(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top)
	{  }

	void CameraController::OnUpdate(float dt){
		CLOVE_PROFILE_FUNCTION();

		glm::vec3 cam_pos = m_cam.GetPosition();
		if      (Input::IsKeyPressed(Key::KEY_W)) cam_pos.y += m_cam_speed * dt * m_zoom_level;
		else if (Input::IsKeyPressed(Key::KEY_S)) cam_pos.y -= m_cam_speed * dt * m_zoom_level;
		if      (Input::IsKeyPressed(Key::KEY_D)) cam_pos.x += m_cam_speed * dt * m_zoom_level;
		else if (Input::IsKeyPressed(Key::KEY_A)) cam_pos.x -= m_cam_speed * dt * m_zoom_level;
		m_cam.SetPosition(cam_pos);

		if (m_rotation == true) {
			float cam_rot = m_cam.GetRotation();
			if (Input::IsKeyPressed(Key::KEY_R)) cam_rot += m_cam_rot_rate * dt;
			else if (Input::IsKeyPressed(Key::KEY_F)) cam_rot -= m_cam_rot_rate * dt;
			if (cam_rot > 360.0f) cam_rot -= 360.0f;
			else if (cam_rot < 0.0f)   cam_rot += 360.0f;
			m_cam.SetRotation(cam_rot);
		}
		m_cam.Update();
	}

	void CameraController::OnEvent(Event& e){
		CLOVE_PROFILE_FUNCTION();
		EventDispatcher dp(e);
		dp.Dispatch<MouseScrolledEvent>(CLOVE_BIND_METHOD_1(CameraController::OnMouseScrolled));
		dp.Dispatch<WindowResizeEvent>(CLOVE_BIND_METHOD_1(CameraController::OnWindowResized));
	}

	void CameraController::CalculateView() {
		m_bounds = { -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level };
		m_cam.SetProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e){
		CLOVE_PROFILE_FUNCTION();
		m_zoom_level -= e.GetYOffset() * 0.5f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		CameraController::CalculateView();
		return false;
	}
	
	bool CameraController::OnWindowResized(WindowResizeEvent& e){
		CLOVE_PROFILE_FUNCTION();
		m_aspect_ratio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		CameraController::CalculateView();
		return false;
	}
}