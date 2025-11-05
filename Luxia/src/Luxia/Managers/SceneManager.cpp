#include "SceneManager.h"


namespace Luxia {
	bool SceneManager::LoadScenesFromPath(const std::filesystem::path& m_path) {
		scene_pool_dir = m_path.string() + "/scenes";

		if (!std::filesystem::exists(scene_pool_dir)) {
			LX_CORE_INFO("Scene Manager: creating asset dir - {}", scene_pool_dir.string());

			// Check if creating folder is successful
			if (!std::filesystem::create_directory(scene_pool_dir)) {
				LX_CORE_ERROR("Scene Manager: failed to create assets dir at - {}", scene_pool_dir.string());
				return false;
			}
		}

		// Load

		return true;
	}

	bool SceneManager::SaveScenes(const std::filesystem::path& m_path) {
		scene_pool_dir = m_path.string() + "/scenes";

		if (!std::filesystem::exists(scene_pool_dir)) {
			LX_CORE_INFO("Scene Manager: creating asset dir - {}", scene_pool_dir.string());

			// Check if creating folder is successful
			if (!std::filesystem::create_directory(scene_pool_dir)) {
				LX_CORE_ERROR("Scene Manager: failed to create assets dir at - {}", scene_pool_dir.string());
				return false;
			}
		}

		return true;
	}

	void SceneManager::Cleanup() {

		// Unload all assets
	}
}