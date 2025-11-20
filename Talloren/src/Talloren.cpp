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

			WeakPtrProxy<Luxia::Scene> s = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Luxia::Scene>());

			auto gasf = asset_manager->GetAssetFileFromPath("cute_ghost/scene.gltf");
			auto tasf = asset_manager->GetAssetFileFromPath("lotr_troll/scene.gltf");
			// auto sasf = asset_manager->GetAssetFileFromPath("shaders/defaultshader.shader");
			auto sasf = asset_manager->Import("shaders/defaultshader.shader", "DefaultShader", "E:/BuiltLuxia/Sandbox/assets/shaders/default.frag", "E:/BuiltLuxia/Sandbox/assets/shaders/default.vert");

			auto ghostModel = asset_manager->CreateAsset<Luxia::IModel>(gasf);
			auto lotrModel = asset_manager->CreateAsset<Luxia::IModel>(tasf);
			auto shader = asset_manager->CreateAsset<Luxia::IShader>(sasf);

			auto mat = std::make_shared<Luxia::Components::Material>(shader);
			
			auto& ghostEntity = s->CreateEntity();
			ghostEntity.position = glm::vec3(0.0f, 0.0f, -10.0f);
			ghostEntity.euler_angles = glm::vec3(-90.0f, 0.0f, 0.0f);
			ghostEntity.AddComponent<Luxia::Components::MeshRenderer>(ghostModel, mat);

			auto& lotrEntity = s->CreateEntity();
			lotrEntity.position = glm::vec3(-4.0f, 0.0f, -10.0f);
			lotrEntity.euler_angles = glm::vec3(0.0f, 45.0f, 0.0f);
			lotrEntity.scale = glm::vec3(0.02f);
			lotrEntity.AddComponent<Luxia::Components::MeshRenderer>(lotrModel, mat);

			auto& camEnt = s->CreateEntity();
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
