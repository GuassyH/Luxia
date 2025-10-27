#pragma once

#include "Event.h"

namespace Luxia {
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float xpos, float ypos) 
			: e_MouseX(xpos), e_MouseY(ypos) {}

		float GetX() const { return e_MouseX; }
		float GetY() const { return e_MouseY; }

		static EventType GetStaticType() { return EventType::MouseMoved; }
		EventType GetEventType() { return GetStaticType(); }

		int GetCategoryFlags() const override {
			return EventCategoryMouse | EventCategoryInput;
		}

	private:
		float e_MouseX, e_MouseY;
	};

};
