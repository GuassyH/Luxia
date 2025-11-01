#pragma once

#include "Core.h"
#include "WeakPtrProxy.h"
#include "Luxia/Events/EventPCH.h"
#include "Luxia/Managers/InputManager.h"
#include "Luxia/Managers/AssetManager.h"

namespace Luxia
{
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& event) = 0;

		void SetEventHandler(std::shared_ptr<EventHandler> handler) { event_handler = handler; }

		Luxia::EventHandler& GetEventHandler() { return *event_handler.lock(); }
	private:
		WeakPtrProxy<EventHandler> event_handler;
	};
}

