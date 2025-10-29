#pragma once

#include "Event.h"

namespace Luxia {
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float xpos, float ypos) 
			: e_MouseX(xpos), e_MouseY(ypos) {}

		float GetX() const { return e_MouseX; }
		float GetY() const { return e_MouseY; }

		GET_EVENT_TYPE(EventType::MouseMoved)
		GET_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "MouseMoveEvent: (" << e_MouseX << ", " << e_MouseY << ")";
			return stream.str();
		}

	private:
		float e_MouseX, e_MouseY;
	};

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset) : e_XOffset(xOffset), e_YOffset(yOffset) {}

		GET_EVENT_TYPE(EventType::MouseScrolled)
		GET_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "MouseScrollEvent: Horizontal = " << e_XOffset << ", Vertical = " << e_YOffset;
			return stream.str();
		}

	private:
		float e_XOffset, e_YOffset;
	};
};
