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
		bool LoadScenePool(std::filesystem::path& projectPath, const std::shared_ptr<AssetManager> m_asset_manager);
		bool SaveScenes(std::filesystem::path& projectPath);
	
		// Used for just assigning the scene, SaveOnSet describes wether the set function should save the last scene
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Assets::SceneFile> m_sceneFile, bool save_on_set = false);
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene, bool save_on_set = false);
		std::shared_ptr<Scene> SetActiveScene(unsigned int index, bool save_on_set = false);

		// Takes care of the loading, starts the scene
		bool LoadScene(unsigned int index);

		bool SaveBuildOrder(std::filesystem::path& projectPath);
		bool LoadBuildOrder(std::filesystem::path& projectPath);
		void AddToBuildOrder(Luxia::GUID& guid);
		void MoveOrderInBuildOrder(int from_index, int to_index);
		void RemoveFromBuildOrder(int index);

		bool running = true; // DONT CHANGE. WILL PAUSE YOUR GAME

		bool HasActiveScene() const { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { 
			if (!active_scene) { return nullptr; }
			return active_scene; 
		}

		bool SaveActiveScene();

		std::unordered_map<Luxia::GUID, std::shared_ptr<Luxia::Assets::SceneFile>> scene_files;
		std::vector<Luxia::GUID> build_order;
	private:
		std::shared_ptr<AssetManager> asset_manager;
		std::shared_ptr<Scene> active_scene;
	};
}
