#pragma once

#include "Luxia/Events/Event.h"
#include "Luxia/Events/EventHandler.h"
#include "Core.h"
#include "WeakPtrProxy.h"

namespace Luxia
{
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& event) = 0;

		virtual void SetEventHandler(std::shared_ptr<EventHandler> handler) { event_handler = handler; }

		Luxia::EventHandler& GetEventHandler() { return *event_handler.lock(); }
	private:
		WeakPtrProxy<EventHandler> event_handler;
	};
}

