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

		std::string title = "Clove Window";
		uint32_t width = 1280, height = 720;
		EventCallbackFn event_callback = nullptr;
		bool fullscreen = false;
	};

	class Window {
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const WindowData& data);
		~Window();

		static Scope<Window> Create(uint32_t width = 1920, uint32_t height = 1080);
		static Scope<Window> Create(const WindowData& data);
		void Update();
		void Destroy();
		bool ShouldClose();

		void* GetHandle();
		unsigned int GetHeight() { return m_data.height; }
		unsigned int GetWidth() { return m_data.width; }
		static void SetVSync(bool enabled);

		float AspectRatio() {
			return (float)(m_data.width) / (float)(m_data.height);
		}

		void SetEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }
	
	private:
		void* m_window_ptr;
		bool m_initialised;
		WindowData m_data;
		Ref<GraphicsContext> m_context;
	};

}

