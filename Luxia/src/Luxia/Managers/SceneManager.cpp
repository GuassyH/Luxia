#include "lxpch.h"
#include "SceneManager.h"

namespace Luxia {
	bool SceneManager::LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager) {
		asset_manager = m_asset_manager;
		
		for (auto& [guid, asset] : asset_manager->GetAssetFiles()) {
			if (asset) {
				if (asset->type == AssetType::SceneType && asset->loaded) {
					scene_file_pool.push_back(asset);
					std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();

					new_scene->LoadFromFile(asset->srcPath);
					scene_asset_pool.push_back(new_scene);

					// LX_CORE_TRACE("SceneManager: Loaded scene {}", asset->srcPath.string());
				}
			}
			else {
				LX_CORE_ERROR("SceneManager: Error assigning scene to scene_pool {}", (uint64_t)guid);
			}
		}

		return true;
	}

	bool SceneManager::SaveScenes() {

		for (auto& asset : scene_file_pool) {
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

	std::shared_ptr<Scene> SceneManager::SetActiveScene(unsigned int index) {
		if (active_scene) { active_scene->Cleanup(); }
		active_scene = scene_asset_pool[index];
		active_scene->Load();

		LX_CORE_INFO("Loaded Scene: {}", 0);

		return active_scene;
	}
	 
	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		active_scene->Cleanup();
	
		for (auto& scene : scene_asset_pool) {
			scene->Cleanup();
		}

		scene_file_pool.clear();
		scene_asset_pool.clear();
	}
}