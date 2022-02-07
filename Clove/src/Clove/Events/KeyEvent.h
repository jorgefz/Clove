#pragma once

#include "Clove/Events/Event.h"

namespace Clove {

	class KeyPressEvent : public Clove::Event {
	private:
		int m_keycode, m_repeats;
	public:
		KeyPressEvent(int keycode, int repeats) : m_keycode(keycode), m_repeats(repeats) {  }
		int GetKeyCode() const { return m_keycode; }
		int GetRepeatCount() const { return m_repeats; }

		static EventType GetStaticEventType() { return EventType::KeyPressed; }
		EventType GetEventType() const override { return GetStaticEventType(); }

		int GetGroupFlags() const override { return EventGroupInput | EventGroupKeyboard; }
		std::string GetName() const override { return "KeyPressEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_keycode;
			return ss.str();
		}
	};

	class KeyReleaseEvent : public Clove::Event {
	private:
		int m_keycode;
	public:
		KeyReleaseEvent(int keycode) : m_keycode(keycode) {  }
		int GetKeyCode() const { return m_keycode; }

		static EventType GetStaticEventType() { return EventType::KeyReleased; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupInput | EventGroupKeyboard; }
		std::string GetName() const override { return "KeyReleaseEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_keycode;
			return ss.str();
		}
	};

	class KeyTypeEvent : public Clove::Event {
	private:
		int m_keycode;
	public:
		KeyTypeEvent(int keycode) : m_keycode(keycode) {  }
		int GetKeyCode() const { return m_keycode; }

		static EventType GetStaticEventType() { return EventType::KeyType; }
		EventType GetEventType() const override { return GetStaticEventType(); }
		int GetGroupFlags() const override { return EventGroupInput | EventGroupKeyboard; }
		std::string GetName() const override { return "KeyTypeEvent"; }
		std::string GetDebugName() const override {
			std::stringstream ss;
			ss << GetName() << ": " << m_keycode;
			return ss.str();
		}
	};

}
