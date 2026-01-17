#include "lxpch.h"
#include "Application.h"
#include "Luxia/Platform/PlatformDefinitions.h"

#include "Luxia/Layers/EventLayer.h"
#include "Luxia/Layers/RenderLayer.h"
#include "Luxia/Layers/GameLayer.h"
#include "Luxia/Layers/ViewportLayer.h"
#include "Luxia/Layers/UILayer.h"

#include "Luxia/Mesh.h"
#include "Luxia/Core/Time.h"

#include "Luxia/Managers/ResourceManager.h"

namespace Luxia
{

	// Create proj manager, renderer, eventhandler, and window on INIT
	Application::Application()
	{
		// Open the project
		m_ProjectManager = std::make_shared<ProjectManager>();
		
		// Initialise event handler
		m_EventHandler = std::make_shared<Luxia::EventHandler>();	
		
		// Create Renderer
		m_Renderer = Luxia::Platform::Rendering::CreateRenderer();

		// Initialise window 
		m_Window = Luxia::Platform::Assets::CreateAppWindow(1920, 1080, "Luxia Application", m_EventHandler);

		// Initialise layer stack 
		m_LayerStack = std::make_shared<LayerStack>();
	
		// Initialize UI
		m_Renderer->GetUIRenderer()->Init();
		
		// Initialise Resources
		ResourceManager::Init();

		LX_CORE_WARN("glBindFrameBuffer: {}", (void*)glBindFramebuffer);
	}

	void Application::CoreStartup() {
		LX_CORE_INFO("Application Started\n");

		// Core Layers
		PushLayer(std::make_shared<Layers::EventLayer>());
		PushLayer(std::make_shared<Layers::GameLayer>());
		PushLayer(std::make_shared<Layers::RenderLayer>());
		PushLayer(std::make_shared<Layers::ViewportLayer>());
		PushLayer(std::make_shared<Layers::UILayer>());
	}

	// Run
	void Application::Run()
	{
		m_ProjectManager->GetSceneManager()->SetActiveScene(0);
		double time_accumulator = 0.0;

		// While the window is running loop
		while (m_Window->isRunning()) {
			Core::Time::get().update();
			m_Window->BeginFrame();
			m_Renderer->GetUIRenderer()->BeginFrame();
			m_EventHandler->DispatchAll(this);

			for (auto& layer : m_LayerStack->m_Layers) {
				if(layer->give_profiler_response) // This field, if true, will be set to false after the render process
					time_accumulator = Luxia::Core::Time::get().GetTimeHP();

				layer->OnUpdate();
				
				if (layer->give_profiler_response) {
					time_accumulator = Luxia::Core::Time::get().GetTimeHP() - time_accumulator;
					std::string text = layer->name + " - Update";
					m_EventHandler->PushEvent(std::make_shared<ProfilerResponseEvent>(text, time_accumulator));
				}
			}
			
			for (auto& layer : m_LayerStack->m_Layers) {
				if(layer->give_profiler_response)
					time_accumulator = Luxia::Core::Time::get().GetTimeHP();

				layer->OnRender();
				
				if (layer->give_profiler_response) {
					time_accumulator = Luxia::Core::Time::get().GetTimeHP() - time_accumulator;
					std::string text = layer->name + " - Render";
					m_EventHandler->PushEvent(std::make_shared<ProfilerResponseEvent>(text, time_accumulator));
					layer->give_profiler_response = false;
				}
			}

			m_Renderer->GetUIRenderer()->EndFrame();
			m_Window->EndFrame();
			eventManager.Clear();

			m_EventHandler->PushEvent(std::make_shared<Luxia::MessageSentEvent>(std::string("End of Frame")));
		}
	}

	void Application::CoreShutdown() {
		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}

		m_ProjectManager->GetSceneManager()->SaveActiveScene();

		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_LayerStack.reset();
		m_Window.reset();
		m_EventHandler.reset();

		m_ProjectManager->CloseProject();
		ResourceManager::Cleanup();

		LX_CORE_ERROR("Application Ended");
	}

	bool Application::OnEvent(Luxia::Event& e) {
		//  Go through each layer from top to bottom and check onEvent
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnEvent(e);
			if (e.isConsumed)
				break;
		}
		if (!e.isConsumed) {
			m_Window->OnEvent(e);
		}
		
		return e.isConsumed;
	}
}
