#pragma once

#include "Luxia/Core/Core.h"
#include "Event.h"

#include <vector>

namespace Luxia {
	class Application;
	
	class LUXIA_API EventHandler {
	public:
		EventHandler() = default; 
		~EventHandler() = default;

		void PushEvent(std::shared_ptr<Event>&& event) {
			event_queue.push_back(std::move(event));
		}

		void DispatchAll(Application* app);

	private:
		std::vector<std::shared_ptr<Event>> event_queue;
	};
}

#define PUSH_EVENT(EventType, ...) this->GetEventHandler().PushEvent(std::make_shared<EventType>(__VA_ARGS__))

