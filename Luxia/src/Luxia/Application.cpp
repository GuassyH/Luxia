#include "Application.h"
#include "PlatformDefinitions.h"

#include "Log.h"

namespace Luxia
{
	Luxia::Application* Application::a_Instance = nullptr;

	Application::Application()
	{
		Application::a_Instance = this;

		m_EventHandler = Luxia::EventHandler();

		m_Window = Luxia::Platform::CreateAppWindow(1920, 1080, "Luxia Application");
		m_Window->SetHandler(m_EventHandler);

		m_LayerStack = std::make_shared<LayerStack>();

		m_Running = true;
	}

	Application::~Application() = default;
	
	

	void Application::Run()
	{
		

		LX_CORE_INFO("Application Started");

		// Set Event Handler for each layer
		for (auto layer : m_LayerStack->m_Layers) {
			layer->SetEventHandler(m_EventHandler);
		}

	
		while (m_Running) {
			// Dispatch Events
			m_EventHandler.DispatchAll(this);
			
			// Begin the Frame
			m_Window->BeginFrame();

			// Loop through layers
			for (auto layer : m_LayerStack->m_Layers) {
				layer->OnUpdate();
			}

			// End the Frame (swap buffers)
			m_Window->EndFrame();
		}

		for(auto layer : m_LayerStack->m_Layers) {
			layer->OnDetach();
		}
		for (auto layer : m_LayerStack->m_Layers) {
			layer->~Layer();
		}

		LX_CORE_ERROR("Application Ended");
	}

	bool Application::OnEvent(Luxia::Event& e) {
		EventDispatcher dispatcher(e);
		if (e.GetEventType() == EventType::WindowClose) {
			m_Running = false; 
			return true;
		}

		for (auto layer : m_LayerStack->m_Layers) {
			layer->OnEvent(e);
			if (e.isConsumed)
				break;
		}

		return e.isConsumed ? true : false;
	}
}
