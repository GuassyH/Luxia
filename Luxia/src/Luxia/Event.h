#pragma once

#include "Core.h"

namespace Luxia {
	class LUXIA_API Event {
	public:
		Event() = default;

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
			EventCategoryInput			= 0b1 << 1,
			EventCategoryKeyboard		= 0b1 << 2,
			EventCategoryMouse			= 0b1 << 3,
			EventCategoryMouseButton	= 0b1 << 4
		};

		bool isConsumed = false;

		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		
	};

}

