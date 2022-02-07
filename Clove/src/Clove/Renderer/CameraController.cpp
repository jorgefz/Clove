#include <clovepch.h>
#include "Clove/Renderer/CameraController.h"
#include "Clove/Input.h"
#include "Clove/KeyCodes.h"

namespace Clove {
	CameraController::CameraController(float aspect_ratio, bool rotation)
		: m_rotation(rotation), m_aspect_ratio(aspect_ratio),
		  m_cam(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
	{

	}

	void CameraController::OnUpdate(float dt){

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
		EventDispatcher dp(e);
		dp.Dispatch<MouseScrolledEvent>(CLOVE_BIND_METHOD_1(CameraController::OnMouseScrolled));
		dp.Dispatch<WindowResizeEvent>(CLOVE_BIND_METHOD_1(CameraController::OnWindowResized));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e){
		m_zoom_level -= e.GetYOffset() * 0.5f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		m_cam.SetProjection(
			-m_aspect_ratio * m_zoom_level,
			m_aspect_ratio * m_zoom_level,
			-m_zoom_level, m_zoom_level
		);
		return false;
	}
	
	bool CameraController::OnWindowResized(WindowResizeEvent& e){
		m_aspect_ratio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		m_cam.SetProjection(
			-m_aspect_ratio * m_zoom_level,
			m_aspect_ratio * m_zoom_level,
			-m_zoom_level, m_zoom_level
		);
		return false;
	}
}