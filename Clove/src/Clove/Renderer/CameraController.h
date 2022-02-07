#pragma once

#include "Clove/Renderer/Camera.h"
#include "Clove/Events/Event.h"
#include "Clove/Events/WindowEvent.h"
#include "Clove/Events/MouseEvent.h"

namespace Clove {

	class CameraController {
	public:
		CameraController(float aspect_ratio, bool rotation = false);
		void OnUpdate(float dt);
		void OnEvent(Event& e);
		Camera& GetCamera() { return m_cam; };
		const Camera& GetCamera() const { return m_cam; };
		void SetZoomLevel(float zoom) { m_zoom_level = zoom; }
		float GetZoomLevel() { return m_zoom_level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_aspect_ratio;
		float m_zoom_level = 1.0f;
		Camera m_cam;
		bool m_rotation;
		float m_cam_speed = 1.0f, m_cam_rot_rate = 45.0f; // degrees per unit time
	};
}