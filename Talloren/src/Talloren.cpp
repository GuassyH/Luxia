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

			WeakPtrProxy<Luxia::Scene> scene = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Luxia::Scene>());

			// Create Shader runtime Asset
			auto MatFile = asset_manager->GetAssetFile<Luxia::Assets::MaterialFile>(asset_manager->GetAssetFileGUID("materials/defaultmat.luxmat"));
			auto SHFile = asset_manager->GetAssetFile<Luxia::Assets::ShaderFile>(MatFile->shader_guid);
			// auto DefaultShader = scene->LoadRuntimeAsset<Luxia::IShader>(SHFile);
			// std::shared_ptr<Luxia::IMaterial> DefaultMat = Luxia::Platform::Assets::CreateMaterial(DefaultShader);

			Luxia::SceneSerializer serializer(scene_manager->scene_files[0]);
			serializer.Deserialize(*scene.lock());

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
