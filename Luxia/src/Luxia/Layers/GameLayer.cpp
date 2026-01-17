#include "lxpch.h"
#include "GameLayer.h"

namespace Luxia::Layers {
	
	void GameLayer::OnAttach() {
		LX_CORE_WARN("GameLayer Attached");
	}
	void GameLayer::OnDetach() {
		LX_CORE_WARN("GameLayer Detached");
	}
	void GameLayer::OnUpdate() {
		std::shared_ptr<Scene> scene = scene_manager->GetActiveScene();

		if (scene) {
			for (auto& [guid, entity] : scene->runtime_entities) {
				auto transform = scene->TryGetFromEntity<Components::Transform>(entity);
				// ONLY UPDATE ROOTS, Children will be updated as a consequence
				if (!transform->HasParent()) {
					transform->UpdateMatrix();
				}
			}
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
	}
}
