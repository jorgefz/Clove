#pragma once
#include "clovepch.h"

#include "Core.h"

#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Clove{

	struct WindowData {
		using EventCallbackFn = std::function<void(Event&)>;

		std::string title;
		unsigned int width, height;
		EventCallbackFn event_callback;
	};

	class Window {
		using EventCallbackFn = std::function<void(Event&)>;
	private:
		GLFWwindow* m_window_ptr;
		bool m_initialised;
		WindowData m_data;

	public:
		Window();
		~Window();

		void Create(GLuint width = 1920, GLuint height = 1080);
		void Update();
		void Destroy();
		bool ShouldClose();

		GLFWwindow* GetWindow();
		GLuint GetHeight() { return m_data.height; }
		GLuint GetWidth() { return m_data.width; }

		float AspectRatio() {
			return static_cast<float>(m_data.width) / static_cast<float>(m_data.height);
		}

		void SetEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }
	};

}

