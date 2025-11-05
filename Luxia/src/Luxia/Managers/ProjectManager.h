#pragma once

#include <filesystem>
#include <iostream>

#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Managers/SceneManager.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Core/Core.h"

namespace Luxia {
	class LUXIA_API ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		bool NewProject(const std::filesystem::path& folder_path);
		bool OpenProject(const std::filesystem::path& folder_path);
		bool SaveProject();
		void CloseProject();
		
		std::shared_ptr<Luxia::AssetManager>& GetAssetManager() { return m_AssetManager; }
		std::shared_ptr<SceneManager>& GetSceneManager() { return m_SceneManager; }

		std::filesystem::path GetAssetPath() { return m_ProjectPath; }
	private:
		std::filesystem::path m_ProjectPath;

		std::shared_ptr<Luxia::AssetManager> m_AssetManager = nullptr;
		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
	};
}
