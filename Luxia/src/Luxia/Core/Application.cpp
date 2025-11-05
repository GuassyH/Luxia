#include "Application.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Layers/InputLayer.h"
#include "Luxia/Layers/RenderLayer.h"
#include "Luxia/Layers/GameLayer.h"

#include "Luxia/Mesh.h"

namespace Luxia
{
	Application::Application()
	{
		// Open the project
		m_ProjectManager = std::make_shared<ProjectManager>();
		m_Renderer = Luxia::Platform::Rendering::CreateRenderer();

		// Initialise event handler
		m_EventHandler = std::make_shared<Luxia::EventHandler>();	

		// Initialise window
		m_Window = Luxia::Platform::Assets::CreateAppWindow(1920, 1080, "Luxia Application");
		m_Window->SetHandler(m_EventHandler);

		// Initialise layerstack
		m_LayerStack = std::make_shared<LayerStack>();
		m_LayerStack->m_Layers.clear();	m_LayerStack->m_Layers.resize(0);

		// Push universal layers (used by all applications) MUST BE DONE IN ORDER BOTTOM TO TOP
		m_LayerStack->PushLayer(std::make_shared<Layers::InputLayer>());
		m_LayerStack->PushLayer(std::make_shared<Layers::GameLayer>());
		m_LayerStack->PushLayer(std::make_shared<Layers::RenderLayer>());
	}



	void Application::Startup() {
		LX_CORE_INFO("Application Started\n");

		// Set Event Handler for each layer
		for (auto& layer : m_LayerStack->m_Layers) 
			layer->SetDeps(m_EventHandler, m_ProjectManager, m_Renderer);

		m_ProjectManager->GetAssetManager()->Load<Assets::TextureAsset>("Lovely.jpg");
		m_ProjectManager->GetAssetManager()->Load<Assets::ModelAsset>("lotr_troll/scene.gltf");
	}

	void Application::Run()
	{
		Startup();

		// While the window is running loop
		while (m_Window->isRunning()) {
			m_Window->BeginFrame();
			m_EventHandler->DispatchAll(this);

			for (auto& layer : m_LayerStack->m_Layers) 
				layer->OnUpdate();
			
			for (auto& layer : m_LayerStack->m_Layers) 
				layer->OnRender();

			m_Window->EndFrame();
			input.Clear();
		}

		Shutdown();
	}

	void Application::Shutdown() {
		// Detach
		for (int i = m_LayerStack->m_Layers.size() - 1; i >= 0; i--) {
			m_LayerStack->m_Layers[i]->OnDetach();
		}

		// Deconstruct
		m_LayerStack->m_Layers.clear();
		m_LayerStack.reset();
		m_Window.reset();
		m_EventHandler.reset();

		m_ProjectManager->CloseProject();

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
