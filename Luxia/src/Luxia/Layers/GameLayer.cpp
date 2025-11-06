#include "lxpch.h"
#include "GameLayer.h"

namespace Luxia::Layers {
	

	void GameLayer::OnAttach() {
		LX_CORE_WARN("GameLayer Attached");
		project_manager->GetAssetManager()->Create<Luxia::Assets::ModelAsset>("cute_ghost/scene.gltf");
	}
	void GameLayer::OnDetach() {
		LX_CORE_WARN("GameLayer Detached");
	}
	void GameLayer::OnUpdate() {
	
	}
	void GameLayer::OnRender() {
	
	}
	void GameLayer::OnEvent(Event& e) {
	
	}
}
