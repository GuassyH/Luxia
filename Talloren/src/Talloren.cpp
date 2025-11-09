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

			std::shared_ptr<Luxia::Assets::ModelAsset> ma = project_manager->GetAssetManager()->Create<Assets::ModelAsset>("cute_ghost/scene.gltf");
			std::shared_ptr<IModel> model = Platform::Assets::CreateModel();
			
			model->LoadFromFile(ma->srcPath);

			std::shared_ptr<IShader> shader = Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");

			auto& entity = s->CreateEntity();
			entity.position = glm::vec3(0.0f, 0.0f, -10.0f);
			entity.euler_angles = glm::vec3(-90.0f, 0.0f, 0.0f);
			entity.AddComponent<Components::MeshRenderer>(s->GetReg(), model, shader);

			auto& camEnt = s->CreateEntity();
			auto& cam = camEnt.AddComponent<Components::Camera>(s->GetReg(), Platform::Assets::CreateCamera(1920, 1080));
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("ExtraLayer Detached");
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