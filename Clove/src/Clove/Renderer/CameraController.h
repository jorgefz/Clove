#pragma once

#include "Clove/Renderer/Camera.h"
#include "Clove/Events/Event.h"
#include "Clove/Events/WindowEvent.h"
#include "Clove/Events/MouseEvent.h"

/*

A handy API for the user to control a camera more easily.

*/

namespace Clove {

	struct CameraBounds {
		float left, right, bottom, top;
		const float GetWidth() const { return right - left; }
		const float GetHeight() const { return top - bottom; }
	};


	class CameraController {
	public:
		CameraController(float aspect_ratio, bool rotation = false);

		void OnUpdate(float dt);
		void OnEvent(Event& e);
		
		void ResizeView(float width, float height);

		Camera& GetCamera() { return m_cam; };
		const Camera& GetCamera() const { return m_cam; };
		float GetZoomLevel() { return m_zoom_level; }
		const CameraBounds& GetBounds() const { return m_bounds; }
		void SetZoomLevel(float zoom) { m_zoom_level = zoom; }

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	
	private:
		float m_aspect_ratio;
		float m_zoom_level = 1.0f;
		CameraBounds m_bounds;
		Camera m_cam;
		bool m_rotation;
		float m_cam_speed = 1.0f, m_cam_rot_rate = 45.0f; // degrees per unit time
	};
}