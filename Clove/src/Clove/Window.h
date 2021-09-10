#pragma once

#include "clovepch.h"

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
		void* m_window_ptr;
		bool m_initialised;
		WindowData m_data;

	public:
		Window();
		~Window();

		void Create(unsigned int width = 1920, unsigned int height = 1080);
		void Update();
		void Destroy();
		bool ShouldClose();

		void* GetWindow();
		unsigned int GetHeight() { return m_data.height; }
		unsigned int GetWidth() { return m_data.width; }

		float AspectRatio() {
			return static_cast<float>(m_data.width) / static_cast<float>(m_data.height);
		}

		void SetEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }
	};

}

