#pragma once

#include "Core.h"
#include "IWindow.h"
#include "Win_Window.h"

#include <iostream>

namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();


		// void PushLayer(std::shared_ptr<Layer> layer) { m_LayerStack->PushLayer(layer); }

		std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		// std::shared_ptr<LayerStack> GetLayerStack() { return nullptr; /*m_LayerStack;*/ }
	protected:
		std::shared_ptr<Luxia::IWindow> m_Window;
		// std::shared_ptr<LayerStack> m_LayerStack;
	};

	// Each client defines this function; editor, runtime, etc.
	Application* CreateApplication();
}

