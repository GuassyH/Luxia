#include "lxpch.h"
#include "EventHandler.h"
#include "Luxia/Core/Application.h"

namespace Luxia {
	void EventHandler::DispatchAll(Application* app) {
		std::vector<std::shared_ptr<Event>> events;
		events.swap(event_queue); // event_queue is now empty; pushes go into it safely

		for (auto& event : events) {
			app->OnEvent(*event);
		}
	}
}