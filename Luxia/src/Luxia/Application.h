#pragma once

#include "Core.h"

#include "Interfaces/IWindow.h"
#include "Systems/InputSystem.h"

#include "Layers/LayerStack.h"
#include "Layers/Layer.h"

#include "Events/EventPCH.h"
#include "Events/WindowEvent.h"

#include "AssetManager.h"

#include <iostream>

namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void PushLayer(std::shared_ptr<Luxia::Layer> layer) { m_LayerStack->PushLayer(layer); }
		bool OnEvent(Luxia::Event& e);

		inline std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		inline std::shared_ptr<Luxia::LayerStack> GetLayerStack() { return m_LayerStack; }
		inline Luxia::EventHandler& GetEventHandler() { return *m_EventHandler; }

		// Each client defines this function; editor, runtime, etc.
	protected:
		std::shared_ptr<Luxia::IWindow> m_Window;
		std::shared_ptr<Luxia::LayerStack> m_LayerStack;
		
		std::shared_ptr<Luxia::EventHandler> m_EventHandler;

		Input::InputSytem& input = Input::InputSytem::Get();
		AssetManager& assetManager = AssetManager::Get();
	};

	Application* CreateApplication();
}

