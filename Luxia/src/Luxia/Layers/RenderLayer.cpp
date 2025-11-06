#include "lxpch.h"
#include "RenderLayer.h"
#include "Luxia/Scene.h"
#include "Luxia/Components/MeshRenderer.h"

namespace Luxia::Layers {

	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");
		std::shared_ptr<Scene> s = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Scene>());

		std::shared_ptr<Luxia::Assets::ModelAsset> ma = project_manager->GetAssetManager()->Create<Luxia::Assets::ModelAsset>("cute_ghost/scene.gltf");
		std::shared_ptr<IModel> model = Platform::Assets::CreateModel();
		model->LoadFromFile(ma->srcPath);

		std::shared_ptr<IShader> shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");

		const auto entity = s->registry.create();
		s->registry.emplace<Luxia::Components::MeshRenderer>(entity, model, shader);
	}
	void RenderLayer::OnDetach() {
		LX_CORE_WARN("RenderLayer Detached");
	}
	void RenderLayer::OnUpdate() {
		cam->UpdateMatrix();
	}
	void RenderLayer::OnRender() {
		bool hasScene = project_manager->GetSceneManager()->HasActiveScene();

		if (hasScene && project_manager->GetSceneManager() != nullptr) {
			std::shared_ptr<Luxia::Scene> scene = project_manager->GetSceneManager()->GetActiveScene();
			
			cam->Render(scene->registry, renderer.lock());
		}
		else {
			LX_CORE_ERROR("No active scene!");
		}
	}
	void RenderLayer::OnEvent(Event& e) {

	}
}
