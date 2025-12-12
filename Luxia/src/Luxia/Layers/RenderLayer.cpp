#include "lxpch.h"
#include "RenderLayer.h"
#include "Luxia/Scene/Scene.h"
#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Components/Camera.h"
#include "Luxia/Components/Transform.h"


namespace Luxia::Layers {

	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");
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

			auto view = scene->GetEntitiesWith<Luxia::Components::Camera>();

			for (auto entity : view) {
				auto& cam = view.get<Luxia::Components::Camera>(entity);
				auto cam_t = scene->TryGetFromEntity<Luxia::Components::Transform>(entity);

				if (cam_t && cam.main) {
					PUSH_EVENT(RenderCameraEvent, cam.Render(scene, renderer.lock()), cam.main);
				}
				else if(!cam_t) { 
					LX_CORE_ERROR("Camera ({}) has no transform!", (int)entity);
				}
			}
		}
	}
	void RenderLayer::OnEvent(Event& e) {

	}
}
