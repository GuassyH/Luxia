#pragma once

#include "Luxia/Managers/AssetManager.h"
#include "Luxia/Managers/SceneManager.h"
#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"

#include <filesystem>
#include <iostream>


namespace Luxia {
	class LUXIA_API ProjectManager {
	public:
		ProjectManager() = default;
		~ProjectManager() = default;

		bool NewProject(const std::filesystem::path& folder_path, const std::string& project_name);
		bool OpenProject(const std::filesystem::path& folder_path);
		void CloseProject();

		bool SaveProjectConfigs();
		bool LoadProjectConfigs();
		
		std::shared_ptr<Luxia::AssetManager>& GetAssetManager() { return m_AssetManager; }
		std::shared_ptr<SceneManager>& GetSceneManager() { return m_SceneManager; }


		const std::filesystem::path& GetProjectPath() { return m_ProjectPath; }
		const std::string& GetProjectName() { return m_ProjectName; }
	private:
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_ConfigPath;

		std::string m_ProjectName;

		std::shared_ptr<Luxia::AssetManager> m_AssetManager = nullptr;
		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
	};
}
