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

						scene_files.push_back(std::move(asset));
					}
				}
			}
		}

		return true;
	}

	bool SceneManager::SaveScenes() {
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
		scene_files.clear();
	}
}