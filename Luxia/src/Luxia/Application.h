#pragma once

#include "Core.h"

#include "IWindow.h"
#include "LayerStack.h"
#include "Layer.h"

#include "Events/Event.h"
#include "Events/EventHandler.h"

#include <iostream>

namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	public:
		virtual ~Application();

		inline static Application& Get() { return *a_Instance; }

		void Run();
		void PushLayer(std::shared_ptr<Luxia::Layer> layer) { m_LayerStack->PushLayer(layer); }
		void OnEvent(Luxia::Event& e);

		inline std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		inline std::shared_ptr<Luxia::LayerStack> GetLayerStack() { return m_LayerStack; }
		inline Luxia::EventHandler& GetEventHandler() { return m_EventHandler; }

		// Each client defines this function; editor, runtime, etc.
	protected:
		static Application* a_Instance;
		
		std::shared_ptr<Luxia::IWindow> m_Window;
		std::shared_ptr<Luxia::LayerStack> m_LayerStack;
		
		Luxia::EventHandler m_EventHandler;

		bool m_Running = true;

		Application();
	};

	Application* CreateApplication();
}

