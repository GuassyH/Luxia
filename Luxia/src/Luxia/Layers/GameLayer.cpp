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

		auto view = scene->GetEntitiesWith<Components::Transform>();
		
		// Should set the parent matrices first, but for now just update all
		for (auto entity : view) {
			auto transform = scene->TryGetFromEntity<Components::Transform>(entity);
			transform->UpdateMatrix();
		}
	}
	void GameLayer::OnRender() {
	
	}
	void GameLayer::OnEvent(Event& e) {
	
	}
}
