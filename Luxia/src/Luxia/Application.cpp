#include "Application.h"
#include "PlatformDefinitions.h"
#include "Log.h"

#include "Layers/InputLayer.h"
#include "Layers/RenderLayer.h"
#include "Layers/GameLayer.h"

namespace Luxia
{
	Application::Application()
	{
		// Initialise event handler
		m_EventHandler = std::make_shared<Luxia::EventHandler>();	
		assetManager.Init();

		// Initialise window
		m_Window = Luxia::Platform::CreateAppWindow(1920, 1080, "Luxia Application");
		m_Window->SetHandler(m_EventHandler);

		// Initialise layerstack
		m_LayerStack = std::make_shared<LayerStack>();
		m_LayerStack->m_Layers.clear();	m_LayerStack->m_Layers.resize(0);

		// Push universal layers (used by all applications) MUST BE DONE IN ORDER BOTTOM TO TOP
		m_LayerStack->PushLayer(std::make_shared<Layers::InputLayer>());
		m_LayerStack->PushLayer(std::make_shared<Layers::GameLayer>());
		m_LayerStack->PushLayer(std::make_shared<Layers::RenderLayer>());
		// m_LayerStack->PushLayer(std::make_shared<Layers::UILayer>());
	}

	void Application::Run()
	{
		LX_CORE_INFO("Application Started");

		// Set Event Handler for each layer
		for (auto& layer : m_LayerStack->m_Layers) {
			layer->SetEventHandler(m_EventHandler);
		}

		std::string path = "C:/dev/Luxia/assets/Test.JPG";
		assetManager.Load<TextureAsset>(path);

		// While the window is running loop
		while (m_Window->isRunning()) {
			// Clear window
			m_Window->BeginFrame();
			m_EventHandler->DispatchAll(this);

			// Go through each layer, from bottom to top, and update
			for (auto& layer : m_LayerStack->m_Layers) {
				layer->OnUpdate();
			}

			// Go through each layer, from bottom to top, and render
			for (auto& layer : m_LayerStack->m_Layers) {
				layer->OnRender();
			}

			// Poll events and swap buffers
			m_Window->EndFrame();
			input.Reset();
		}


		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}
		
		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_LayerStack.reset();
		m_Window.reset();
		m_EventHandler.reset();
		assetManager.Finish();
		
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
