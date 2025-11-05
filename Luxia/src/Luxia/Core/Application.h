#pragma once

#include "Luxia/Core/Core.h"

#include "Luxia/Rendering/API/IWindow.h"
#include "Luxia/Rendering/API/IRenderer.h"

#include "Luxia/Managers/InputManager.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Managers/ProjectManager.h"

#include "Luxia/Events/EventDefs.h"
#include "Luxia/Layers/LayerStack.h"
#include "Luxia/Layers/Layer.h"


namespace Luxia
{
	// The application base class
	class LUXIA_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void CoreStartup();
		virtual void Startup() {}
		void Run();
		virtual void Shutdown();

		void PushLayer(std::shared_ptr<Luxia::Layer> layer) { 
			m_LayerStack->PushLayer(layer, m_EventHandler, m_ProjectManager, m_Renderer); 
		}
		bool OnEvent(Luxia::Event& e);

		inline std::shared_ptr<Luxia::IWindow> GetWindow() { return m_Window; }
		inline std::shared_ptr<Luxia::LayerStack> GetLayerStack() { return m_LayerStack; }

		std::shared_ptr<Luxia::ProjectManager> GetProjectManager() { return m_ProjectManager; }
		std::shared_ptr<Luxia::EventHandler> GetEventHandler() { return m_EventHandler; }
		std::shared_ptr<Luxia::Rendering::IRenderer> GetRenderer() { return m_Renderer; }
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

