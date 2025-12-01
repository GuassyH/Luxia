#include <Luxia.h>
#include <filesystem>

#include "Luxia/Core/EntryPoint.h"
#include "ViewportLayer.h"

namespace Talloren::Layer {

	class ExtraLayer : public Luxia::Layer {
	public:
		ExtraLayer() = default;
		~ExtraLayer() = default;
		
		virtual void OnAttach() override { 
			LX_CORE_WARN("ExtraLayer Attached");

			WeakPtrProxy<Luxia::Scene> scene = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Luxia::Scene>());

			auto& camEnt = scene->CreateEntity();
			auto& cam = camEnt.AddComponent<Luxia::Components::Camera>(2560, 1440);
			cam.main = true;
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("ExtraLayer Detached");
		}
		virtual void OnUpdate() override {}
		virtual void OnRender() override {}
		virtual void OnEvent(Luxia::Event& event) override {}
	};
}


namespace Talloren {
	class TallorenApp : public Luxia::Application {
	public:
		TallorenApp() = default;
		~TallorenApp() = default;

		virtual void Startup() override {
			// Push extra layers used
			PushLayer(std::make_shared<Talloren::Layer::ExtraLayer>());
			PushLayer(std::make_shared<Talloren::Layer::ViewportLayer>());
			m_Window->SetTitle(m_ProjectManager->GetProjectName());
		}
	};
}

std::unique_ptr<Luxia::Application> Luxia::CreateApplication() {
	std::unique_ptr<Luxia::Application> app = std::make_unique<Talloren::TallorenApp>();

	std::filesystem::path new_path = "E:/BuiltLuxia/Sandbox";
	app->GetProjectManager()->OpenProject(new_path);

	return app;
};
