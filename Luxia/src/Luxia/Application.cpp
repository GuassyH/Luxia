#include "Application.h"
#include "PlatformDefinitions.h"

namespace Luxia
{
	Application::Application()
	{
		m_Window = Platform::CreateWindow();
		m_Window->Create(1920, 1080, "Luxia Application");
	}

	Application::~Application() = default;
	

	void Application::Run()
	{
		while (!m_Window->ShouldClose()) {
			// Begin the Frame
			m_Window->BeginFrame();

			// Loop through layers
			// for (auto& layer : *m_LayerStack) {
			// 	layer->OnUpdate();
			// }

			// End the Frame (swap buffers)
			m_Window->EndFrame();
		}
	}
}
