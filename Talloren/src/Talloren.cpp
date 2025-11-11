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

			std::shared_ptr<Luxia::Assets::ModelAsset> gma = project_manager->GetAssetManager()->Create<Luxia::Assets::ModelAsset>("cute_ghost/scene.gltf");
			std::shared_ptr<Luxia::Assets::ModelAsset> lma = project_manager->GetAssetManager()->Create<Luxia::Assets::ModelAsset>("lotr_troll/scene.gltf");
			std::shared_ptr<Luxia::IModel> ghostModel = Luxia::Platform::Assets::CreateModel();
			std::shared_ptr<Luxia::IModel> lotrModel = Luxia::Platform::Assets::CreateModel();

			ghostModel->LoadFromFile(gma->srcPath);
			lotrModel->LoadFromFile(lma->srcPath);

			std::shared_ptr<Luxia::IShader> shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");
			std::shared_ptr<Luxia::Components::Material> mat = std::make_shared<Luxia::Components::Material>(shader);
			
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
			auto& cam = camEnt.AddComponent<Luxia::Components::Camera>(Luxia::Platform::Assets::CreateCamera(1920, 1080));
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
			GetWindow()->SetTitle("Island Application");
		}
	};

}

std::unique_ptr<Luxia::Application> Luxia::CreateApplication() {
	std::unique_ptr<Luxia::Application> app = std::make_unique<Talloren::TallorenApp>();

	// LOAD FIRST
	std::filesystem::path new_path = "E:/BuiltLuxia/NewProject";
	app->GetProjectManager()->OpenProject(new_path);

	return app;
};
