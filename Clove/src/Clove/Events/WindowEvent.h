#pragma once

#include "Event.h"

namespace Clove {

	class WindowCloseEvent : public Clove::Event {
	public:
		WindowCloseEvent() {  }

		static EventType GetStaticEventType() { return EventType::WindowClose; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroup::EventGroupWindow; }
		std::string GetName() const override { return "WindowCloseEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName();
			return ss.str();
		}
	};

	class WindowResizeEvent : public Clove::Event {
	private:
		GLuint m_width, m_height;
	public:
		WindowResizeEvent(GLuint width, GLuint height) : m_width(width), m_height(height) {  }
		GLuint GetWidth() { return m_width; }
		GLuint GetHeight() { return m_height; }

		static EventType GetStaticEventType() { return EventType::WindowResize; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroup::EventGroupWindow; }
		std::string GetName() const override { return "WindowResizeEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_width << 'x' << m_height;
			return ss.str();
		}
	};
}