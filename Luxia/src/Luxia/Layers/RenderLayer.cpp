#include "lxpch.h"
#include "RenderLayer.h"
#include "Luxia/Scene.h"
#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Components/Camera.h"
#include "Luxia/Components/Transform.h"

namespace Luxia::Layers {

	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");
		std::shared_ptr<Scene> s = project_manager->GetSceneManager()->SetActiveScene(std::make_shared<Scene>());

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
	void RenderLayer::OnDetach() {
		LX_CORE_WARN("RenderLayer Detached");
	}
	void RenderLayer::OnUpdate() {
	
	}
	void RenderLayer::OnRender() {
		bool hasScene = project_manager->GetSceneManager()->HasActiveScene();

		if (hasScene && project_manager->GetSceneManager() != nullptr) {
			std::shared_ptr<Luxia::Scene> scene = project_manager->GetSceneManager()->GetActiveScene();

			auto view = scene->registry.view<Luxia::Components::Camera>();

			for (auto entity : view) {
				auto& cam = view.get<Luxia::Components::Camera>(entity);
				cam.camera->UpdateMatrix();
				cam.camera->Render(scene->registry, renderer.lock());
			}
		}
	}
	void RenderLayer::OnEvent(Event& e) {

	}
}
