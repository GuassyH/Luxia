#pragma once

#include "Luxia/Core/Core.h"

#include <filesystem>

namespace Luxia {
	class LUXIA_API SceneManager {
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void Cleanup();

		bool LoadScenesFromPath(const std::filesystem::path& m_path);
		bool SaveScenes(const std::filesystem::path& m_path);
	private:
		std::filesystem::path scene_pool_dir;
	};
}
