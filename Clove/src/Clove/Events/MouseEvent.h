#pragma once

#include "Clove/Events/Event.h"

namespace Clove {

	class MouseMovedEvent : public Clove::Event {
	private:
		float m_x, m_y;
	public:
		MouseMovedEvent(float x, float y) : m_x(x), m_y(y) {  }
		float GetX() { return m_x; }
		float GetY() { return m_y; }

		static EventType GetStaticEventType() { return EventType::MouseMoved; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupMouse | EventGroupInput; }
		std::string GetName() const override { return "MouseMovedEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_x << ' ' << m_y;
			return ss.str();
		}
	};

	class MouseScrolledEvent : public Clove::Event {
	private:
		float m_xoff, m_yoff;
	public:
		MouseScrolledEvent(float xoff, float yoff) : m_xoff(xoff), m_yoff(yoff) {  }
		float GetXOffset() { return m_xoff; }
		float GetYOffset() { return m_yoff; }

		static EventType GetStaticEventType() { return EventType::MouseScrolled ; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupMouse | EventGroupInput; }
		std::string GetName() const override { return "MouseScrolledEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_xoff << ' ' << m_yoff;
			return ss.str();
		}
	};

	class MouseButtonPressedEvent : public Clove::Event {
	private:
		int m_button;
	public:
		MouseButtonPressedEvent(int button) : m_button(button) {  }
		int GetMouseButton() { return m_button; }

		static EventType GetStaticEventType() { return EventType::MouseButtonPressed; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupMouse | EventGroupInput; }
		std::string GetName() const override { return "MouseButtonPressed"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_button;
			return ss.str();
		}
	};

	class MouseButtonReleasedEvent : public Clove::Event {
	private:
		int m_button;
	public:
		MouseButtonReleasedEvent(int button) : m_button(button) {  }
		int GetMouseButton() { return m_button; }

		static EventType GetStaticEventType() { return EventType::MouseButtonReleased; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupMouse | EventGroupInput; }
		std::string GetName() const override { return "MouseButtonReleased"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_button;
			return ss.str();
		}
	};

}
