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

};

