#pragma once

#include "Luxia/Events/Event.h"
#include "Luxia/Events/EventHandler.h"
#include "Core.h"

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

		virtual void SetEventHandler(Luxia::EventHandler& handler) { event_handler = &handler; }

		Luxia::EventHandler& GetEventHandler() { return *event_handler; }
	private:
		Luxia::EventHandler* event_handler;
	};
}

#define PUSH_EVENT(EventType, ...) this->GetEventHandler().PushEvent(std::make_shared<EventType>(__VA_ARGS__))
#define IS_EVENT(EventType, event) \
	{ EventDispatcher dispatcher(event); \
	dispatcher.Dispatch<EventType>([](EventType& e) { return true; }); }