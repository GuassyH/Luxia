#include "lxpch.h"
#include "SceneManager.h"

namespace Luxia {
	bool SceneManager::LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager) {
		asset_manager = m_asset_manager;

		for (auto& [guid, metafile] : asset_manager->GetMetaFilePool()) {
			if (metafile) {
				if (metafile->type == AssetType::SceneType) {
					if (asset_manager->GetAssetFilePool().contains(metafile->guid)) {
						auto asset = asset_manager->GetAssetFile<Assets::SceneFile>(metafile->guid);
						std::shared_ptr<Scene> new_scene = std::make_shared<Scene>();

						new_scene->LoadFromFile(asset);

						scene_map[asset] = new_scene;
						
						LX_CORE_TRACE("SceneManager: Loaded scene {}", metafile->assetPath.string());
					}
				}
			}
			else {
				LX_CORE_ERROR("SceneManager: Error assigning scene to scene_pool {}", (uint64_t)guid);
			}
		}

		return true;
	}

	bool SceneManager::SaveScenes() {
		for (auto& [file, scene] : scene_map) {
			if (scene) {
				scene->SaveToFile(file);
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
		LX_CORE_ERROR("SceneManager: SetActiveScene by index not implemented yet");

		return active_scene;
	}
	 
	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		active_scene->Cleanup();

		for (auto& [file, scene] : scene_map) {
			scene->Cleanup();
		}

		scene_map.clear();
	}
}