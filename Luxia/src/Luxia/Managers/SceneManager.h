#pragma once

#include "Luxia/Core.h"
#include <filesystem>

namespace Luxia {
	class LUXIA_API SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void Cleanup() {} 

		bool LoadScenesFromPath(const std::filesystem::path& m_path) { scene_pool_path = m_path; return true; }
		bool SaveScenes(const std::filesystem::path& m_path) { return true; }
	private:
		std::filesystem::path scene_pool_path;
	};
}
