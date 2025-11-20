#pragma once

#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Managers/SceneManager.h"
#include "Luxia/Core/Core.h"

#include <filesystem>
#include <iostream>


namespace Luxia {
	class LUXIA_API ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		bool NewProject(const std::filesystem::path& folder_path, const std::string& project_name);
		bool OpenProject(const std::filesystem::path& folder_path);
		bool SaveProjectConfigs();
		bool LoadProjectConfigs();
		void CloseProject();
		
		std::shared_ptr<Luxia::AssetManager>& GetAssetManager() { return m_AssetManager; }
		std::shared_ptr<SceneManager>& GetSceneManager() { return m_SceneManager; }

		std::filesystem::path GetProjectPath() { return m_ProjectPath; }
		std::string GetProjectName() { return m_ProjectName; }
	private:
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_ConfigPath;

		std::string m_ProjectName;

		std::shared_ptr<Luxia::AssetManager> m_AssetManager = nullptr;
		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
	};
}
