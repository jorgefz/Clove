#pragma once

#include "Clove/Events/Event.h"

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
		unsigned int m_width, m_height;
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {  }
		unsigned int GetWidth() { return m_width; }
		unsigned int GetHeight() { return m_height; }

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