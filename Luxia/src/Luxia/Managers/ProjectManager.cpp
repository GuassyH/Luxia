#include "ProjectManager.h"

namespace Luxia {

	bool ProjectManager::NewProject(const std::filesystem::path& folder_path) {

		// Check if there is already a folder at that path
		if (std::filesystem::exists(folder_path)) {
			LX_CORE_WARN("Project Manager: folder already at path - {}", folder_path.string());
			return false;
		}

		// Check if creating folder is successful
		if (!std::filesystem::create_directory(folder_path)) {
			LX_CORE_ERROR("Project Manager: failed to create project dir at - {}", folder_path.string());
			return false;
		}

		// Create a ProjectFile (.lux?)
		m_ProjectPath = folder_path;
		
		m_AssetManager = std::make_shared<Luxia::AssetManager>();
		m_SceneManager = std::make_shared<Luxia::SceneManager>();

		// Try to save project
		if (!SaveProject()) {
			LX_CORE_ERROR("Project Manager: failed to save project at - {}", folder_path.string());
			return false;
		}

		LX_CORE_INFO("Project Manager: created project at {}", folder_path.string());
		return true;
	}

	bool ProjectManager::OpenProject(const std::filesystem::path& folder_path) {
		// Open the JSON project file
		// Read the information (saved assets etc)
		if (!std::filesystem::exists(folder_path)) {
			LX_CORE_ERROR("Project Manager: project doesnt exist - {}", folder_path.string());
			return false;
		}

		m_ProjectPath = folder_path;

		m_AssetManager = std::make_shared<Luxia::AssetManager>();
		m_SceneManager = std::make_shared<Luxia::SceneManager>();
		

		m_AssetManager->LoadAssetPoolFromPath(folder_path);
		m_SceneManager->LoadScenesFromPath(folder_path);

		LX_CORE_INFO("Project Manager: opened project at {}", folder_path.string());
		return true;
	}

	bool ProjectManager::SaveProject() {
		bool complete;

		complete = 
			m_AssetManager->SaveAssetPool(m_ProjectPath) &&
			m_SceneManager->SaveScenes(m_ProjectPath);

		return complete;
	}

	void ProjectManager::CloseProject() {
		m_AssetManager->Cleanup();
		m_AssetManager.reset();
	}
};