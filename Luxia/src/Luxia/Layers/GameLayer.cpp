#include "lxpch.h"
#include "GameLayer.h"
#include "Luxia/Components/Collider.h"

namespace Luxia::Layers {
	
	void GameLayer::OnAttach() {
		LX_CORE_WARN("GameLayer Attached");
		if (scene_manager->running) {
			if (scene_manager->LoadScene(0)) {
				auto scene = scene_manager->GetActiveScene();
				scene->Start();
			}
		}
	}
	void GameLayer::OnDetach() {
		LX_CORE_WARN("GameLayer Detached");
	}
	void GameLayer::OnUpdate() {
		std::shared_ptr<Scene> scene = scene_manager->GetActiveScene();

		if (scene) {

			auto transform_view = scene->GetEntitiesWith<Components::Transform>();
			for (auto entity : transform_view) {
				auto& transform = scene->GetFromEntity<Components::Transform>(entity);
				if (!transform.HasParent()) {
					transform.UpdateMatrix();
				}
			}

			// Update Colliders (dirty check?)
			auto collider_view = scene->GetEntitiesWith<Components::Collider>();
			for (auto& entity : collider_view) {
				auto& col = scene->GetFromEntity<Components::Collider>(entity);
				if (col.enabled && col.colliderShape) {
					if (col.colliderShape->scale != col.scale) {
						col.InitCollider();
					}
				}
			}

			if (scene_manager->running && !scene->isStarted)
				scene->Start();

			// Should check for should update
			if(scene_manager->running && scene->isStarted)
				scene->Update();
		}
	}
	void GameLayer::OnRender() {

	}
	void GameLayer::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<ProfilerRequestEvent>([&](ProfilerRequestEvent& event) {
			give_profiler_response = true;
			return false; // Check each event type and update input
			});

		dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& event) {
			auto scene = scene_manager->GetActiveScene();
			if (scene)
				scene->End();
			scene_manager->running = false;
			return false;
			});
	}
}
