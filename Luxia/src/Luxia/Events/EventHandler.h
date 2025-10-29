#pragma once

#include "Luxia/Core.h"
#include "Event.h"

#include <iostream>
#include <vector>
#include <memory>
#include <xmemory>

class Application;

namespace Luxia {
	class LUXIA_API EventHandler {
	public:
		EventHandler() = default; 
		~EventHandler() = default;

		void PushEvent(std::shared_ptr<Event>&& event) {
			event_queue.push_back(std::move(event));
		}

		void DispatchAll(Application* app) {}

	private:
		std::vector<std::shared_ptr<Event>> event_queue;
	};
}

