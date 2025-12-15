#include <Luxia.h>
#include <filesystem>

#include "Luxia/Core/EntryPoint.h"
#include "EditorLayer.h"

#include "Luxia/Scene/SceneSerializer.h"

namespace Talloren::Layers {

	class ExtraLayer : public Luxia::Layer {
	public:
		ExtraLayer() = default;
		~ExtraLayer() = default;
		virtual void OnAttach() override { 
			LX_WARN("ExtraLayer Attached");

			std::shared_ptr<Luxia::Scene> scene = project_manager->GetSceneManager()->SetActiveScene(0);

			if (scene_manager->scene_files[0] != nullptr) {
				Luxia::SceneSerializer serializer(scene_manager->scene_files[0]);
				if (scene_manager->scene_files[0]->assets[0] != nullptr);
					serializer.Deserialize();
			}
			
		}
		virtual void OnDetach() override {
			LX_WARN("ExtraLayer Detached");
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
			PushLayer(std::make_shared<Talloren::Layers::EditorLayer>());
			PushLayer(std::make_shared<Talloren::Layers::ExtraLayer>()); // Used for quick client side testing
			m_Window->SetTitle("Luxia Editor");
			std::filesystem::path iconpath = m_ProjectManager->GetProjectPath() / "editor" / "LuxiaLogo.png";
			m_Window->SetIcon(iconpath);
		}
	};
}

std::unique_ptr<Luxia::Application> Luxia::CreateApplication() {
	std::unique_ptr<Luxia::Application> app = std::make_unique<Talloren::TallorenApp>();

	std::filesystem::path new_path = "E:/BuiltLuxia/Sandbox";
	app->GetProjectManager()->OpenProject(new_path);

	return app;
};
