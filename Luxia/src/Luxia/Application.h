#pragma once

#include "Core.h"

#include "IWindow.h"
#include "LayerStack.h"
#include "Layer.h"

#include <iostream>


namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	private:
		static Application* a_Instance;
		
		std::shared_ptr<Luxia::IWindow> m_Window;
		std::shared_ptr<Luxia::LayerStack> m_LayerStack;
	public:
		Application();
		virtual ~Application();

		inline static Application& Get() { return *a_Instance; }

		void Run();
		void PushLayer(std::shared_ptr<Luxia::Layer> layer) { m_LayerStack->PushLayer(layer); }
		void OnEvent(/* Event e */) {}

		inline std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		inline std::shared_ptr<Luxia::LayerStack> GetLayerStack() { return m_LayerStack; }
	};

	// Each client defines this function; editor, runtime, etc.
	Application* CreateApplication();
}

