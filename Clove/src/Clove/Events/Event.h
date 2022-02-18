#pragma once
#include "clovepch.h"

namespace Clove {
	

	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased, KeyType,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventGroup {
		None = 0,
		EventGroupInput =    (1 << 0), // mouse click + scroll, keys, etc
		EventGroupKeyboard = (1 << 1), // keys
		EventGroupMouse =    (1 << 2), // mouse click + scroll
		EventGroupWindow =   (1 << 3), // window resize + close
	};

	// Forward declarations
	class EventDispatcher;
	class Event;

	/*
	* An Event reflects a certain change of state in a program.
	* Each Event only contains data relevant to that change of state.
	* E.g. on window resize, it contains the new height & width.
	* E.g. on key press, it contains the key code.
	* When an event is 'dispatched', it means it is dealt with.
	* The change of state is propagated through the program as it reacts to it.
	* E.g. mouse click on a specific position? It clicks a button.
	*/
	class Event {
	private:
		friend class Clove::EventDispatcher; // friend can access Event's private & protected members
	public:
		bool handled = false;
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual int GetGroupFlags() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string GetDebugName() const = 0;
		bool IsInGroup(EventGroup group) { return GetGroupFlags() & group; }
		bool Handled() { return handled; }
	};

	/*
	* Takes an event, and calls the appropriate function to deal with it.
	*/
	class EventDispatcher {
	private:
		Event& m_event;
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_event(event) {  }
		
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if(m_event.GetEventType() == T::GetStaticEventType()) {
				/*
				* Function called takes an action given the event
				* E.g. Key press event -> move player
				*/
				m_event.handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	};
}
