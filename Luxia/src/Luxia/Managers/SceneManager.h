#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Core/WeakPtrProxy.h"

#include <filesystem>
#include <iostream>
#include <unordered_set>


namespace Luxia {
	class Scene;
	
	class LUXIA_API SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void Cleanup();
		bool LoadScenePool(const std::shared_ptr<AssetManager> m_asset_manager);
		bool SaveScenes();
	
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene);

		bool HasActiveScene() const { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { return active_scene; }
	private:
		std::unordered_set<std::shared_ptr<Assets::AssetFile>> scene_pool;
		std::shared_ptr<AssetManager> asset_manager;
		std::shared_ptr<Scene> active_scene;
	};
}
