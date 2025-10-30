#pragma once


#include "Event.h"

namespace Luxia {
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		GET_EVENT_TYPE(EventType::WindowClose)
		GET_EVENT_CATEGORY(EventCategoryWindow | EventCategoryInput)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Close Event";
			return stream.str();
		}
	};



	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int w, int h) : width(w), height(h) {}

		GET_EVENT_TYPE(EventType::WindowResize)
		GET_EVENT_CATEGORY(EventCategoryWindow)

		int GetX() { return width; }
		int GetY() { return height; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Resized: (" << GetX() << ", " << GetY() << ")";
			return stream.str();
		}

	private:
		int width, height;
	};



	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(int x, int y) : xPos(x), yPos(y) {}

		GET_EVENT_TYPE(EventType::WindowResize)
		GET_EVENT_CATEGORY(EventCategoryWindow)

		int GetX() { return xPos; }
		int GetY() { return yPos; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Moved: (" << GetX() << ", " << GetY() << ")";
			return stream.str();
		}

	private:
		int xPos, yPos;
	};

	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() {}

		GET_EVENT_TYPE(EventType::WindowFocus)
		GET_EVENT_CATEGORY(EventCategoryWindow)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Focused";
			return stream.str();
		}

	};

	class WindowLoseFocusEvent : public Event {
	public:
		WindowLoseFocusEvent() {}

		GET_EVENT_TYPE(EventType::WindowLostFocus)
		GET_EVENT_CATEGORY(EventCategoryWindow)

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Lost Focus";
			return stream.str();
		}

	};
};

