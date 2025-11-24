#include "lxpch.h"
#include "SceneManager.h"
#include "Luxia/Scene.h"

namespace Luxia {
	bool SceneManager::LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager) {
		asset_manager = m_asset_manager;
		
		for (auto& [guid, asset] : asset_manager->GetAssetFiles()) {
			if (asset) {
				if (asset->type == AssetType::SceneType && !scene_pool.contains(asset)) {
					scene_pool.insert(asset);
				}
			}
			else {
				LX_CORE_ERROR("SceneManager: Error assigning scene to scene_pool {}", (uint64_t)guid);
			}
		}

		return true;
	}

	bool SceneManager::SaveScenes() {

		for (auto& asset : scene_pool) {
			if (asset) {
				asset->Save();
			}
			else {
				LX_CORE_ERROR("SceneManager: Error saving scene");
			}
		}

		return true;
	}

	std::shared_ptr<Scene> SceneManager::SetActiveScene(std::shared_ptr<Scene> m_scene) {
		if (active_scene) { active_scene->Cleanup(); }
		active_scene = m_scene;
		active_scene->Load();

		LX_CORE_INFO("Loaded Scene: {}", 0);

		return active_scene;
	}

	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		active_scene->Cleanup();
	
		scene_pool.clear();
	}
}