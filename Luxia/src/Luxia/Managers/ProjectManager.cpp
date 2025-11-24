#include "lxpch.h"
#include "ProjectManager.h"

namespace Luxia {

	bool ProjectManager::NewProject(const std::filesystem::path& folder_path, const std::string& project_name) {
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


		m_ProjectPath = folder_path.lexically_normal();
		m_ProjectName = project_name;
		m_ConfigPath = (folder_path / "config.lux").lexically_normal();
		
		m_AssetManager = std::make_shared<Luxia::AssetManager>();
		m_SceneManager = std::make_shared<Luxia::SceneManager>();

		// Try to save project
		if (!SaveProjectConfigs()) {
			LX_CORE_ERROR("Project Manager: failed to save project at - {}", folder_path.string());
			return false;
		}

		LX_CORE_INFO("Project Manager: created project at - {}", folder_path.string());
		return true;
	}

	bool ProjectManager::OpenProject(const std::filesystem::path& folder_path) {
		if (!std::filesystem::exists(folder_path)) {
			LX_CORE_ERROR("Project Manager: project doesnt exist - {}", folder_path.string());
			return false;
		}

		m_ProjectPath = folder_path.lexically_normal();
		m_ConfigPath = (folder_path / "config.lux").lexically_normal();


		m_AssetManager = std::make_shared<Luxia::AssetManager>();
		m_SceneManager = std::make_shared<Luxia::SceneManager>();
		
		// Try to load project
		if (!LoadProjectConfigs()) {
			LX_CORE_ERROR("Error loading project configs at: {}", m_ConfigPath.string());
		}

		LX_CORE_INFO("Project Manager: opened project at {}", folder_path.string());
		return true;
	}

	// Should be YAML!
	bool ProjectManager::SaveProjectConfigs() {
		bool complete = m_AssetManager->SaveAssetPool();

		std::ofstream outfile(m_ConfigPath);
		if (!outfile.is_open()) { return false; }

		outfile << "path=" << m_ProjectPath.string() << "\n";
		outfile << "name=" << m_ProjectName << "\n";

		outfile.close();

		return complete;
	}

	// Should be YAML
	bool ProjectManager::LoadProjectConfigs() {
		std::ifstream infile(m_ConfigPath);
		if (!infile.is_open()) { return false; }

		int success = 0;
		std::string line;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("path=", 0) == 0) {				
				m_ProjectPath = line.substr(5);;
				success++;
			}
			else if (line.rfind("name=", 0) == 0) {	
				m_ProjectName = line.substr(5);
				success++;
			}
		}

		infile.close();
		
		// if all lines werent found, return false
		if (success != 2) { return false; }

		bool complete = (m_AssetManager->LoadAssetPool(m_ProjectPath) &&
			m_SceneManager->LoadScenePool(m_AssetManager));

		return complete;
	}

	void ProjectManager::CloseProject() {
		SaveProjectConfigs();

		m_SceneManager->Cleanup();
		m_AssetManager->Cleanup();

		m_SceneManager.reset();
		m_AssetManager.reset();
	}
};