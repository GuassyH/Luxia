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
	
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Assets::SceneFile> m_sceneFile);
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene);
		std::shared_ptr<Scene> SetActiveScene(unsigned int index);


		bool SaveBuildOrder(std::filesystem::path& projectPath);
		bool LoadBuildOrder(std::filesystem::path& projectPath);
		void AddToBuildOrder(Luxia::GUID& guid);
		void MoveOrderInBuildOrder(int from_index, int to_index);
		void RemoveFromBuildOrder(int index);

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
