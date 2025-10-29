#pragma once

#include "Luxia/Core.h"
#include <iostream>
#include <ostream>
#include <sstream>

namespace Luxia {
	// The type of event (defined in other headers)
	enum class EventType {
		None = 0,
		AppRender,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// The events category
	enum EventCategory {			// Bitmap
		None						= 0b0,
		EventCategoryApplication	= 0b1 << 0,
		EventCategoryWindow			= 0b1 << 1,
		EventCategoryInput			= 0b1 << 2,
		EventCategoryKeyboard		= 0b1 << 3,
		EventCategoryMouse			= 0b1 << 4,
		EventCategoryMouseButton	= 0b1 << 5
	};
}

#define GET_EVENT_TYPE(type) \
	static EventType GetStaticType() { return type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }

#define GET_EVENT_CATEGORY(category) \
virtual int GetCategoryFlags() const override { return category; }

namespace Luxia
{
	class LUXIA_API Event {
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual bool IsInCategory(EventCategory category) const {
			return (GetCategoryFlags() & category) == category;
		}

		virtual std::string GetDebug() { std::ostringstream ostr; ostr << "Debug Event Info Not Found"; return ostr.str(); }

		bool isConsumed = false;
	protected:
	};

}


namespace Luxia {
	class EventDispatcher {
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {
		}
		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.isConsumed = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

};

#define LX_BIND_EVENT_FN(fn) [this](auto& e) { return this->fn(e); }