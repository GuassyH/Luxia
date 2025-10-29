#include "Application.h"
#include "PlatformDefinitions.h"
#include "TestLayer.h"

#include "Events/MouseEvent.h"
#include "Log.h"

namespace Luxia
{
	Luxia::Application* Application::a_Instance = nullptr;

	Application::Application()
	{
		Application::a_Instance = this;

		m_EventHandler = Luxia::EventHandler();

		m_Window = Luxia::Platform::CreateAppWindow(1920, 1080, "Luxia Application");

		m_LayerStack = std::make_shared<LayerStack>();
		m_LayerStack->PushLayer(std::make_shared<Luxia::Layers::TestLayer>());
	}

	Application::~Application() = default;
	
	void Application::Run()
	{
		LX_CORE_INFO("Application Started");

		
		for (auto layer : m_LayerStack->m_Layers) {
			layer->SetEventHandler(m_EventHandler);
		}
	
		Luxia::MouseMoveEvent moveEvent(100.0f, 200.0f);
		LX_CORE_TRACE(moveEvent.GetDebug());

		if(moveEvent.GetEventType() == Luxia::EventType::MouseMoved) {
			LX_CORE_TRACE("MouseMoveEvent Type Verified");
		}

		while (!m_Window->ShouldClose()) {
			// Begin the Frame
			m_Window->BeginFrame();

			// Loop through layers
			for (auto layer : m_LayerStack->m_Layers) {
				layer->OnUpdate();
			}

			// End the Frame (swap buffers)
			m_Window->EndFrame();
		}

		LX_CORE_ERROR("Application Ended");
	}

	void Application::OnEvent(Luxia::Event& e) {
		for (auto layer : m_LayerStack->m_Layers) {
			// layer->OnEvent();
			if (e.isConsumed)
				break;
		}
	}
}
