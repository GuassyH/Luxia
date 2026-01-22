
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
		m_SceneManager->SaveScenes(m_ProjectPath);
		m_AssetManager->SaveAssetPool(m_ProjectPath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		
		out << YAML::Key << "ProjectPath" << YAML::Value << m_ProjectPath.string();
		out << YAML::Key << "ProjectName" << YAML::Value << m_ProjectName;

		out << YAML::EndMap;

		std::ofstream outfile(m_ConfigPath);
		if (!outfile.is_open()) { return false; }

		outfile << out.c_str();

		outfile.close();

		return true;
	}

	// Should be YAML
	bool ProjectManager::LoadProjectConfigs() {
		try {
			YAML::Node config = YAML::LoadFile(m_ConfigPath.string());

			// Check if missing
			m_ProjectPath = config["ProjectPath"].as<std::string>();
			m_ProjectName = config["ProjectName"].as<std::string>();
		}
		catch (const YAML::Exception& ex) {
			std::cerr << "YAML error: " << ex.what() << "\n";
		}

		m_AssetManager->LoadAssetPool(m_ProjectPath);
		m_SceneManager->LoadScenePool(m_ProjectPath, m_AssetManager);

		return true;
	}

	void ProjectManager::CloseProject() {
		SaveProjectConfigs();

		m_SceneManager->Cleanup();
		m_AssetManager->Cleanup();

		m_SceneManager.reset();
		m_AssetManager.reset();
	}
};