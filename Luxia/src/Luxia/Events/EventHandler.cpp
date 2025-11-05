#include "EventHandler.h"
#include "Luxia/Core/Application.h"


namespace Luxia {
	void EventHandler::DispatchAll(Application* app) {
		for (auto& event : event_queue) {
			app->OnEvent(*event);
		}
		event_queue.clear();
	}
}