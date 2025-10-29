#include "Application.h"
#include "PlatformDefinitions.h"
#include "Events/MouseEvent.h"
#include "Log.h"

namespace Luxia
{
	Luxia::Application* Application::a_Instance = nullptr;

	Application::Application()
	{
		Application::a_Instance = this;
		
		m_EventHandler = std::make_shared<Luxia::EventHandler>();	

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


		// ======= MAIN LOOP =======

		while (m_Running) {
			m_EventHandler->DispatchAll(this);
			
			m_Window->BeginFrame();

			for (auto layer : m_LayerStack->m_Layers) {
				layer->OnUpdate();
			}

			m_Window->EndFrame();
		}

		// ======= MAIN LOOP =======


		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}
		
		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_Window->Close();
		m_Window.reset();

		m_EventHandler.reset();

		LX_CORE_ERROR("Application Ended");
	}

	bool Application::OnEvent(Luxia::Event& e) {
		EventDispatcher dispatcher(e);
		m_Running = !dispatcher.Dispatch<WindowCloseEvent>([](WindowCloseEvent& e) { return true; });

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
