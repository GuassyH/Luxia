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
		WindowMoveEvent(float x, float y) : xPos(x), yPos(y) {}

		GET_EVENT_TYPE(EventType::WindowResize)
		GET_EVENT_CATEGORY(EventCategoryWindow)

		float GetX() { return xPos; }
		float GetY() { return yPos; }

		std::string GetDebug() override {
			std::ostringstream stream; stream << "Window Moved: (" << GetX() << ", " << GetY() << ")";
			return stream.str();
		}

	private:
		float xPos, yPos;
	};
};

