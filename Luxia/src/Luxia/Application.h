#pragma once

#include "Core.h"

#include "Rendering/API/IWindow.h"
#include "Rendering/API/IRenderer.h"

#include "Managers/InputManager.h"
#include "Managers/AssetManager.h"
#include "Managers/ProjectManager.h"

#include "Events/EventPCH.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"


#include <iostream>

namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		virtual void Startup();
		void Run();
		virtual void Shutdown();

		void PushLayer(std::shared_ptr<Luxia::Layer> layer) { m_LayerStack->PushLayer(layer); }
		bool OnEvent(Luxia::Event& e);

		inline std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		inline std::shared_ptr<Luxia::LayerStack> GetLayerStack() { return m_LayerStack; }
		std::shared_ptr<Luxia::ProjectManager> GetProjectManager() { return m_ProjectManager; }
		// Each client defines this function; editor, runtime, etc.
	protected:
		std::shared_ptr<Luxia::IWindow> m_Window;
		std::shared_ptr<Luxia::LayerStack> m_LayerStack;
		std::shared_ptr<Luxia::EventHandler> m_EventHandler;
		Input::InputManager& input = Input::InputManager::Get();
		
		std::shared_ptr<Luxia::ProjectManager> m_ProjectManager;
		std::shared_ptr<Luxia::Rendering::IRenderer> m_Renderer;
	};

	Application* CreateApplication();

}

