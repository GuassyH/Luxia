#pragma once

#include <iostream>
#include <Luxia/Core.h>
#include <Luxia/Events/Event.h>
#include <Luxia/Events/EventHandler.h>
#include "Luxia/Events/WindowEvent.h"
#include "WeakPtrProxy.h"

namespace Luxia
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual int Create(int width, int height, const std::string& title = "Luxia") = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Close() = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual bool ResizeEvent(WindowResizeEvent& e) = 0;
		virtual bool MoveEvent(WindowMoveEvent& e) = 0;

		virtual void OnEvent(Event& e) = 0;

		virtual void SetHandler(std::shared_ptr<EventHandler> handler) { event_handler = handler; }
		virtual EventHandler& GetEventHandler() { return *event_handler.lock(); }
	
	protected:
		WeakPtrProxy<EventHandler> event_handler;
	};
}
