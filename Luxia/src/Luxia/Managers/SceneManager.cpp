#include "lxpch.h"
#include "SceneManager.h"
#include "Luxia/Scene/SceneSerializer.h"

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

	std::shared_ptr<Scene> SceneManager::SetActiveScene(std::shared_ptr<Assets::SceneFile> m_sceneFile) {
		if (active_scene) { 
			SceneSerializer oldserializer(active_scene, asset_manager);
			oldserializer.Serialize();
		}
		active_scene = m_sceneFile;

		SceneSerializer serializer(m_sceneFile, asset_manager);
		serializer.Deserialize();

		LX_CORE_INFO("Loaded Scene: {}", 0);

		return GetActiveScene();
	}

	std::shared_ptr<Scene> SceneManager::SetActiveScene(unsigned int index) {
		if (!scene_files[index]) { LX_CORE_ERROR("SceneManager: Set Scene failed, no scene at index: {}", index); return nullptr; }
		
		SetActiveScene(scene_files[index]);

		return GetActiveScene();
	}
	 
	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		active_scene->Unload();
		scene_files.clear();
	}
}