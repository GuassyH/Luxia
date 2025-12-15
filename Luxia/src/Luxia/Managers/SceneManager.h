#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Core/WeakPtrProxy.h"

#include <filesystem>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "Luxia/Scene/Scene.h"

namespace Luxia {
	class Scene;
	
	class LUXIA_API SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void Cleanup();
		bool LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager);
		bool SaveScenes();
	
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Assets::SceneFile> m_sceneFile);
		std::shared_ptr<Scene> SetActiveScene(unsigned int index);

		bool HasActiveScene() const { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { 
			if (!active_scene) { LX_CORE_ERROR("SceneManager: No Active Scene!"); return nullptr; }
			return std::dynamic_pointer_cast<Scene>(active_scene->assets[0]); 
		}

		std::vector<std::shared_ptr<Luxia::Assets::SceneFile>> scene_files;
	private:
		std::shared_ptr<AssetManager> asset_manager;
		std::shared_ptr<Assets::SceneFile> active_scene;
	};
}
