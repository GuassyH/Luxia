#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Core/WeakPtrProxy.h"

#include <filesystem>
#include <iostream>
#include <unordered_set>

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
	
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene);
		std::shared_ptr<Scene> SetActiveScene(unsigned int index);

		bool HasActiveScene() const { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { return active_scene; }
	private:
		std::unordered_map<std::shared_ptr<Assets::SceneFile>, std::shared_ptr<Luxia::Scene>> scene_map;

		std::shared_ptr<AssetManager> asset_manager;
		std::shared_ptr<Scene> active_scene;
	};
}
