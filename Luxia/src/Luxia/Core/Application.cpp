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
#include "Luxia/Physics/PhysicsSystem.h"
namespace Luxia
{

	// Create proj manager, renderer, eventhandler, and window on INIT
	Application::Application()
	{
		// Open the project
		m_ProjectManager = std::make_shared<ProjectManager>();
		
		// Initialize Physics
		Physics::PhysicsSystem::Initialize();

		// Create Renderer
		m_Renderer = Luxia::Platform::Rendering::CreateRenderer();

		// Initialise window 
		m_Window = Luxia::Platform::Assets::CreateAppWindow(1920, 1080, "Luxia Application");

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
		Luxia::EventHandler& event_handler = Luxia::EventHandler::get();
		Luxia::Core::Time& time = Luxia::Core::Time::get();

		// While the window is running loop
		while (m_Window->isRunning()) {
			time.update();
			m_Window->BeginFrame();
			m_Renderer->GetUIRenderer()->BeginFrame();
			event_handler.DispatchAll(this);

			for (auto& layer : m_LayerStack->m_Layers) {
				if(layer->give_profiler_response) // This field, if true, will be set to false after the render process
					time_accumulator = time.GetTimeHP();

				layer->OnUpdate();
				
				if (!layer->give_profiler_response) continue;
				time_accumulator = time.GetTimeHP() - time_accumulator;
				std::string text = layer->name + " - Update";
				event_handler.PushEvent(std::make_shared<ProfilerResponseEvent>(text, time_accumulator));
			}
			
			for (auto& layer : m_LayerStack->m_Layers) {
				if(layer->give_profiler_response)
					time_accumulator = time.GetTimeHP();

				layer->OnRender();
				
				if (!layer->give_profiler_response) continue;
				time_accumulator = time.GetTimeHP() - time_accumulator;
				std::string text = layer->name + " - Render";
				event_handler.PushEvent(std::make_shared<ProfilerResponseEvent>(text, time_accumulator));
				layer->give_profiler_response = false;
			}

			m_Renderer->GetUIRenderer()->EndFrame();
			m_Window->EndFrame();
			eventManager.Clear();

			event_handler.PushEvent(std::make_shared<Luxia::MessageSentEvent>(std::string("End of Frame")));
		}
	}

	void Application::CoreShutdown() {
		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}


		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_LayerStack.reset();
		m_Window.reset();
		// Remove events? 

		m_ProjectManager->CloseProject();
		ResourceManager::Cleanup();

		Physics::PhysicsSystem::Shutdown();

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
