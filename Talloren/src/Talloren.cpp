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

			// Makesure scene manager exists and scene exists
			auto sceneMgr = GetSceneManager();
			if (!sceneMgr) { LX_CORE_WARN("No SceneManager available yet"); return; }
			if (sceneMgr->scene_files.empty()) { LX_CORE_WARN("No scenes available"); return; }
			sceneMgr->SetActiveScene(0);
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
			std::filesystem::path iconpath = "resources/LuxiaLogo.png";
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
