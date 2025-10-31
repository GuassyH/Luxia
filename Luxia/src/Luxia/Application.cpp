#include "Application.h"
#include "PlatformDefinitions.h"
#include "Events/MouseEvent.h"
#include "Log.h"

#include "Layers/InputLayer.h"
#include "Layers/RenderLayer.h"

namespace Luxia
{
	Application* Application::a_Instance = nullptr;

	Application::Application()
	{
		Application::a_Instance = this;
		
		m_EventHandler = std::make_shared<Luxia::EventHandler>();	

		m_Window = Luxia::Platform::CreateAppWindow(1920, 1080, "Luxia Application");
		m_Window->SetHandler(m_EventHandler);

		m_LayerStack = std::make_shared<LayerStack>();
		m_LayerStack->m_Layers.clear();	m_LayerStack->m_Layers.resize(0);

		m_LayerStack->PushLayer(std::make_shared<InputLayer>());
		m_LayerStack->PushLayer(std::make_shared<RenderLayer>());

	}

	Application::~Application() = default;


	void Application::Run()
	{
		LX_CORE_INFO("Application Started");
		Input::InputSytem& input = Input::InputSytem::Get();

		// Set Event Handler for each layer
		for (auto& layer : m_LayerStack->m_Layers) {
			layer->SetEventHandler(m_EventHandler);
		}


		// ======= MAIN LOOP =======

		while (m_Window->isRunning()) {
			m_EventHandler->DispatchAll(this);

			m_Window->BeginFrame();

			for (auto& layer : m_LayerStack->m_Layers) {
				// Input Layer should be first to update
				layer->OnUpdate();
			}

			m_Window->EndFrame();
			input.Reset();
		}

		// ======= ========= =======


		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}
		
		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_Window.reset();
		m_EventHandler.reset();
		
		LX_CORE_ERROR("Application Ended");
	}


	bool Application::OnEvent(Luxia::Event& e) {

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
