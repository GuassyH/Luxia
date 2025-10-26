#include "Application.h"
#include "PlatformDefinitions.h"
#include "TestLayer.h"

#include "Log.h"

namespace Luxia
{
	Luxia::Application* Application::a_Instance = nullptr;

	Application::Application()
	{
		Application::a_Instance = this;

		m_Window = Luxia::Platform::CreateAppWindow();
		m_Window->Create(1920, 1080, "Luxia Application");

		m_LayerStack = std::make_shared<LayerStack>();
		m_LayerStack->PushLayer(std::make_shared<Luxia::Layers::TestLayer>());
	}

	Application::~Application() = default;
	
	void Application::Run()
	{
		LX_CORE_INFO("Application Started");

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
}
