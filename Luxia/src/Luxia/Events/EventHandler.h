#pragma once

#include "Luxia/Core/Core.h"
#include "Event.h"

#include <vector>

namespace Luxia {
	class Application;
	
	class LUXIA_API EventHandler {
	private:
		EventHandler() {
		}
	public:
		EventHandler(const EventHandler&) = delete;
		EventHandler& operator=(const EventHandler&) = delete;
		static EventHandler& get() {
			static EventHandler instance;
			return instance;
		}
		~EventHandler() = default;


		void PushEvent(std::shared_ptr<Event>&& event) {
			event_queue.push_back(std::move(event));
		}

		void DispatchAll(Application* app);

	private:
		std::vector<std::shared_ptr<Event>> event_queue;
	};
}

#define PUSH_EVENT(EventType, ...) Luxia::EventHandler::get().PushEvent(std::make_shared<EventType>(__VA_ARGS__))

