#pragma once

#include "Luxia/Core/Core.h"

#include <filesystem>
#include <iostream>


namespace Luxia {
	class Scene;
	
	class LUXIA_API SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void Cleanup();

		bool LoadScenesFromPath(const std::filesystem::path& m_path);
		bool SaveScenes(const std::filesystem::path& m_path);
	
		std::shared_ptr<Scene> SetActiveScene(std::shared_ptr<Scene> m_scene);

		bool HasActiveScene() { return active_scene != nullptr; }
		std::shared_ptr<Scene> GetActiveScene() { return active_scene; }
	private:
		std::filesystem::path scene_pool_dir;
		std::shared_ptr<Scene> active_scene;
	};
}
