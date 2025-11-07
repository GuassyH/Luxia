#include <Luxia.h>
#include <filesystem>

namespace Luxia {
	class ExtraLayer : public Layer {
	public:
		ExtraLayer() = default;
		~ExtraLayer() = default;

		virtual void OnAttach() override { 
			LX_CORE_WARN("ExtraLayer Attached");

			WeakPtrProxy<Scene> s = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Scene>());

			std::shared_ptr<Luxia::Assets::ModelAsset> ma = project_manager->GetAssetManager()->Create<Luxia::Assets::ModelAsset>("cute_ghost/scene.gltf");
			std::shared_ptr<IModel> model = Platform::Assets::CreateModel();
			
			model->LoadFromFile(ma->srcPath);

			std::shared_ptr<IShader> shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");

			entt::entity entity = s->registry.create();
			auto& et = s->registry.emplace<Luxia::Components::Transform>(entity);
			et.ent_id = entity;
			et.AddComponent<Luxia::Components::MeshRenderer>(s->registry, model, shader);

			entt::entity camEnt = s->registry.create();
			auto& ct = s->registry.emplace<Luxia::Components::Transform>(camEnt);
			ct.ent_id = camEnt;
			ct.AddComponent<Luxia::Components::Camera>(s->registry, Platform::Assets::CreateCamera(1920, 1080));
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("ExtraLayer Detached");
			// project_manager = nullptr;
			// event_handler = nullptr;
			// renderer = nullptr;
		}
		virtual void OnUpdate() override {}
		virtual void OnRender() override {}
		virtual void OnEvent(Event& event) override {}
	};
}


class TallorenApp : public Luxia::Application {
public:
	TallorenApp() = default;
	~TallorenApp() = default;

	virtual void Startup() override {
		// Push extra layers used
		PushLayer(std::make_shared<Luxia::ExtraLayer>());		
		GetWindow()->SetTitle("Island Application");
	}
};

std::unique_ptr<Luxia::Application> Luxia::CreateApplication(){
	std::unique_ptr<Luxia::Application> app = std::make_unique<TallorenApp>();

	// LOAD FIRST
	std::filesystem::path new_path = "E:/BuiltLuxia/NewProject";
	app->GetProjectManager()->OpenProject(new_path);

	return app;
};