#include "lxpch.h"
#include "GameLayer.h"

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
			// This should always update, even if playing is paused
			for (auto& [guid, entity] : scene->runtime_entities) {
				auto transform = scene->TryGetFromEntity<Components::Transform>(entity);
				// ONLY UPDATE ROOTS, Children will be updated as a consequence
				if (!transform->HasParent()) {
					transform->UpdateMatrix();
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
