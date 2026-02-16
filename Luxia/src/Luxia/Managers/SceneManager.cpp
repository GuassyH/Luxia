#include "lxpch.h"
#include "SceneManager.h"
#include "Luxia/Scene/SceneSerializer.h"

namespace Luxia {
	bool SceneManager::LoadScenePool(std::filesystem::path& projectPath, const std::shared_ptr<AssetManager> m_asset_manager) {
		asset_manager = m_asset_manager;

		scene_files.clear();

		for (auto& [guid, assetfile] : asset_manager->GetAssetFilePool()) {
			if (assetfile) {
				if (assetfile->type == AssetType::Scene) {
					auto scenefile = asset_manager->GetAssetFile<Assets::SceneFile>(assetfile->guid);
					scene_files[scenefile->guid] = scenefile;
				}
			}
		}

		LoadBuildOrder(projectPath);

		return true;
	}


	bool SceneManager::SaveScenes(std::filesystem::path& projectPath) {
		return SaveBuildOrder(projectPath);
	}


	// Set and Get Functions 

	std::shared_ptr<Scene> SceneManager::SetActiveScene(std::shared_ptr<Assets::SceneFile> m_sceneFile, bool save_on_set) {
		if (active_scene && save_on_set) { 
			SceneSerializer oldserializer(active_scene->scene_file.lock(), asset_manager);
			oldserializer.Serialize();
			active_scene->Unload();
		}
		if (active_scene)
			active_scene->Unload();

		// Set new Active Scene
		active_scene = std::dynamic_pointer_cast<Scene>(m_sceneFile->assets[0]);

		if (!active_scene) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, could not cast to Scene"); return nullptr; }

		SceneSerializer serializer(m_sceneFile, asset_manager);
		active_scene->Load();
		serializer.Deserialize();

		LX_CORE_INFO("Loaded Scene: {}", (uint64_t)active_scene->guid);

		return GetActiveScene();
	}

	std::shared_ptr<Scene> SceneManager::SetActiveScene(std::shared_ptr<Scene> m_scene, bool save_on_set) {
		if (active_scene && save_on_set) {
			SceneSerializer oldserializer(active_scene->scene_file.lock(), asset_manager);
			oldserializer.Serialize();
		}
		if(active_scene)
			active_scene->Unload();

		// Set new Active Scene
		active_scene = m_scene;

		if (!active_scene) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, could not cast to Scene"); return nullptr; }
		if (!active_scene->scene_file) { LX_CORE_ERROR("SceneManager: Set Active Scene failed, Scene has no SceneFile"); return nullptr; }

		SceneSerializer serializer(m_scene->scene_file.lock(), asset_manager);
		active_scene->Load();
		serializer.Deserialize();

		LX_CORE_INFO("Loaded Scene: {}", (uint64_t)active_scene->guid);

		return GetActiveScene();
	}


	std::shared_ptr<Scene> SceneManager::SetActiveScene(unsigned int index, bool save_on_set) {
		if (index >= build_order.size()) {
			LX_CORE_ERROR("SceneManager: Set Scene failed, index {} out of range (size={})", index, build_order.size());
			return nullptr;
		}
		if (!scene_files.contains(build_order[index])) {
			LX_CORE_ERROR("SceneManager: Set SceneFile failed, no scene at index: {}", index);
			return nullptr;
		}

		return SetActiveScene(scene_files.find(build_order[index])->second, save_on_set);
	}
	 
	bool SceneManager::SaveActiveScene() {
		if (running) {
			LX_CORE_ERROR("SceneManager: Cant save scene while it is running!");
			return false;
		}

		if (!active_scene || !active_scene->scene_file) {
			LX_CORE_ERROR("SceneManager: Save Active Scene failed, no active scene");
			return false;
		}
		SceneSerializer serializer(active_scene->scene_file.lock(), asset_manager);
		serializer.Serialize();
		LX_CORE_INFO("Saved Active Scene");

		return true;
	}

	bool SceneManager::LoadScene(unsigned int index) {
		if (build_order.size() >= index) return false;

		if (active_scene)
			active_scene->End();

		auto scene = SetActiveScene(index);
		if (!scene) return false;

		scene->Start();

		return true;
	}

	void SceneManager::Cleanup() {
		LX_CORE_TRACE("Scene manager cleaned up");

		if(active_scene)
			active_scene->Unload();
		
		scene_files.clear(); 
		build_order.clear();
		active_scene.reset();
	}


	bool SceneManager::LoadBuildOrder(std::filesystem::path& projectPath) {
		// Load build order
		build_order.clear();
		std::filesystem::path buildconfig_p = projectPath / "buildconfig.lux";

		if (std::filesystem::exists(buildconfig_p)) {
			try {
				YAML::Node config = YAML::LoadFile(buildconfig_p.string());

				auto order = config["BuildOrder"];

				if (order && order.IsSequence()) {
					for (auto guid : order) {
						Luxia::GUID g = Luxia::GUID(guid.as<uint64_t>());
						if (scene_files.contains(g)) {
							AddToBuildOrder(g);
							LX_CORE_INFO("SceneManager: Added {} as {} in build order", guid.as<uint64_t>(), (build_order.size() - 1));
						}
						else {
							LX_CORE_ERROR("SceneManager: Error adding {} to build order", guid.as<uint64_t>(), (build_order.size() - 1));
						}
					}
				}
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				return false;
			}
		}
		else {
			SaveBuildOrder(projectPath);
		}
		return true;
	}


	bool SceneManager::SaveBuildOrder(std::filesystem::path& projectPath) {
		std::filesystem::path buildconfig_p = projectPath / "buildconfig.lux";

		YAML::Emitter out;

		if (!out.good()) return false;

		// Emit
		out << YAML::BeginMap;
		out << YAML::Key << "BuildOrder" << YAML::Value << YAML::BeginSeq;

		// Add the sequence
		for (auto& guid : build_order) {
			out << YAML::Value << (uint64_t)guid;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream outfile(buildconfig_p.string());

		if (!outfile.is_open()) return false;

		// Add to file
		outfile << out.c_str();

		return true;
	}

	void SceneManager::AddToBuildOrder(Luxia::GUID& guid) {
		if (std::find(build_order.begin(), build_order.end(), guid) == build_order.end()) {
			build_order.push_back(guid);
		}
	}

	void SceneManager::MoveOrderInBuildOrder(int from_index, int to_index) {
		if (from_index < 0 || from_index >= build_order.size()) return;
		if (to_index < 0 || to_index >= build_order.size()) return;
		if (from_index == to_index) return;

		auto guid = build_order[from_index];
		build_order.erase(build_order.begin() + from_index);
		build_order.insert(build_order.begin() + to_index, guid);
	}

	void SceneManager::RemoveFromBuildOrder(int index) {
		if (index < 0 || index >= build_order.size()) return;
		build_order.erase(build_order.begin() + index);
	}


}