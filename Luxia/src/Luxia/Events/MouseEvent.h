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

		float GetX() { return e_XOffset; }
		float GetY() { return e_YOffset; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "MouseScrollEvent: Horizontal = " << GetX() << ", Vertical = " << GetY();
			return stream.str();
		}

	private:
		float e_XOffset, e_YOffset;
	};

	

	class MouseButtonPressEvent : public Event {
	public:
		MouseButtonPressEvent(int button) :  e_Button(button) {}

		GET_EVENT_TYPE(EventType::MouseButtonPressed)
		GET_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton) 

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Mouse Button Pressed: " << e_Button;
			return stream.str();
		}

	private:
		int e_Button;
	};

	class MouseButtonReleaseEvent : public Event {
	public:
		MouseButtonReleaseEvent(int button) : e_Button(button) {}

		GET_EVENT_TYPE(EventType::MouseButtonReleased)
		GET_EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Mouse Button Pressed: " << e_Button;
			return stream.str();
		}

	private:
		int e_Button;
	};
};
