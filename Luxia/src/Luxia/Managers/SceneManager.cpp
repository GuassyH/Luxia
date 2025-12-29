#include "lxpch.h"
#include "SceneManager.h"
#include "Luxia/Scene/SceneSerializer.h"

namespace Luxia {
	bool SceneManager::LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager) {
		asset_manager = m_asset_manager;

		for (auto& [guid, assetfile] : asset_manager->GetAssetFilePool()) {
			if (assetfile) {
				if (assetfile->type == AssetType::SceneType) {
					auto asset = asset_manager->GetAssetFile<Assets::SceneFile>(assetfile->guid);
					scene_files.push_back(std::move(asset));
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
			SceneSerializer oldserializer(active_scene->scene_file.lock(), asset_manager);
			oldserializer.Serialize();
			active_scene->Unload();
		}

		// Set new Active Scene
		active_scene = std::dynamic_pointer_cast<Scene>(m_sceneFile->assets[0]);

		if (!active_scene) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, could not cast to Scene"); return nullptr; }

		SceneSerializer serializer(m_sceneFile, asset_manager);
		serializer.Deserialize();

		LX_CORE_INFO("Loaded Scene: {}", 0);

		return GetActiveScene();
	}

	std::shared_ptr<Scene> SceneManager::SetActiveScene(std::shared_ptr<Scene> m_scene) {
		if (active_scene) {
			SceneSerializer oldserializer(active_scene->scene_file.lock(), asset_manager);
			oldserializer.Serialize();
			active_scene->Unload();
		}

		// Set new Active Scene
		active_scene = m_scene;

		if (!active_scene) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, could not cast to Scene"); return nullptr; }
		if (!active_scene->scene_file) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, Scene has no SceneFile"); return nullptr; }

		SceneSerializer serializer(m_scene->scene_file.lock(), asset_manager);
		serializer.Deserialize();

		LX_CORE_INFO("Loaded Scene: {}", 0);

		return GetActiveScene();
	}


	std::shared_ptr<Scene> SceneManager::SetActiveScene(unsigned int index) {
		if (index >= scene_files.size()) {
			LX_CORE_ERROR("SceneManager: Set Scene failed, index {} out of range (size={})", index, scene_files.size());
			return nullptr;
		}
		if (!scene_files[index]) {
			LX_CORE_ERROR("SceneManager: Set Scene failed, no scene at index: {}", index);
			return nullptr;
		}
		return SetActiveScene(scene_files[index]);
	}
	 
	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		if(active_scene)
			active_scene->Unload();
		
		scene_files.clear();
	}
}