#pragma once

#include "Luxia/Events/EventDefs.h"
#include <Luxia/Core/Core.h>
#include "Luxia/Core/WeakPtrProxy.h"
#include "Luxia/Core/Core.h"

namespace Luxia
{
	class LUXIA_API IWindow // A base window class
	{
	public:
		virtual ~IWindow() = default;

		// Basic runtime functions
		virtual int Create(const int width, const int height, const std::string& title = "Luxia") = 0;
		virtual void BeginFrame() = 0; // Poll events and clear window
		virtual void EndFrame() = 0; // Swap buffers
		virtual void Close() = 0;

		virtual void OnEvent(Event& e) = 0;
		void SetHandler(std::shared_ptr<EventHandler> handler) { event_handler = handler; }
		EventHandler& GetEventHandler() const { return *event_handler.lock(); }

		virtual void SetTitle(const std::string& title) = 0;
		inline bool IsFocused() const { return focused; }
		inline bool isRunning() const { return running; }
	protected:
		// Basic functions for what to do with each eventtype, can be overridden
		virtual bool ResizeEvent(WindowResizeEvent& e) {
			m_Width = e.GetX();
			m_Height = e.GetY();
			return true;
		}		
		virtual bool MoveEvent(WindowMoveEvent& e) {
			m_PosX = e.GetX();
			m_PosY = e.GetY();
			return true;
		}		
		virtual bool FocusEvent(WindowFocusEvent& e) {
			focused = true;
			return true;
		}		
		virtual bool LoseFocusEvent(WindowLoseFocusEvent& e) {
			focused = false;
			return true;
		}		
		virtual bool CloseEvent(WindowCloseEvent& e) {
			Close();
			return true;
		}		

		WeakPtrProxy<EventHandler> event_handler = nullptr;

		int m_Width = 0;
		int m_Height = 0;

		int m_MonitorWidth = 0;
		int m_MonitorHeight = 0;

		int m_PosX = 0;
		int m_PosY = 0;

		bool focused = true;
		bool running = true;
	};
}
