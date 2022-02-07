#pragma once

#include "clovepch.h"

#include "Clove/Events/Event.h"
#include "Clove/Events/WindowEvent.h"
#include "Clove/Events/KeyEvent.h"
#include "Clove/Events/MouseEvent.h"

#include "Clove/Renderer/GraphicsContext.h"

namespace Clove{

	struct WindowData {
		using EventCallbackFn = std::function<void(Event&)>;

		std::string title;
		unsigned int width, height;
		EventCallbackFn event_callback;
	};

	class Window {
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(unsigned int width = 1920, unsigned int height = 1080);
		~Window();

		static Window* Create(unsigned int width = 1920, unsigned int height = 1080);
		void Update();
		void Destroy();
		bool ShouldClose();

		void* GetHandle();
		unsigned int GetHeight() { return m_data.height; }
		unsigned int GetWidth() { return m_data.width; }

		float AspectRatio() {
			return static_cast<float>(m_data.width) / static_cast<float>(m_data.height);
		}

		void SetEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }
	
	private:
		void* m_window_ptr;
		bool m_initialised;
		WindowData m_data;
		GraphicsContext* m_context;
	};

}

