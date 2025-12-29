#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Core/WeakPtrProxy.h"

#include <filesystem>
#include <iostream>
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
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene);
		std::shared_ptr<Scene> SetActiveScene(unsigned int index);

		bool HasActiveScene() const { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { 
			if (!active_scene) { return nullptr; }
			return active_scene; 
		}

		std::vector<std::shared_ptr<Luxia::Assets::SceneFile>> scene_files;
		// std::vector<Luxia::GUID> scene_order; /* Not implemented */
	private:
		std::shared_ptr<AssetManager> asset_manager;
		std::shared_ptr<Scene> active_scene;
	};
}
