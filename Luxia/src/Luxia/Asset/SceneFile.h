#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Scene/Scene.h"

namespace Luxia::Assets {
	class LUXIA_API SceneFile : public AssetFile {
	public:
		SceneFile() = default;
		SceneFile(const std::filesystem::path m_scenePath)
			: scene_path(m_scenePath.lexically_normal()) {
			type = Luxia::AssetType::Scene;
		}
		SceneFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~SceneFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::Scene;
			guid = GUID();

			assetPath = m_assetPath.lexically_normal();
			scene_path = assetPath.lexically_normal();

			std::shared_ptr<Scene> scene = std::make_shared<Scene>();
			scene->name = m_assetPath.filename().replace_extension("").string();
			scene->assetFileGUID = guid;
			scene->guid = GUID();

			assets.push_back(scene);
			loaded = true;

			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::Key << "AssetFileGUID" << YAML::Value << (uint64_t)guid;
			out << YAML::Key << "Name" << YAML::Value << scene->name;
			out << YAML::Key << "GUID" << YAML::Value << (uint64_t)scene->guid;
			out << YAML::EndMap;

			std::ofstream outfile(scene_path.string());
			outfile << out.c_str();
			outfile.close();

			Save(m_assetPath);
			return true;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::Scene;

			assetPath = m_assetPath.lexically_normal();
			scene_path = assetPath.lexically_normal();

			std::shared_ptr<Asset> scene = std::make_shared<Scene>();

			// This loads asset information, such as name and GUID
			// The rest (entities) is loaded through asset serializer
			try {
				YAML::Node config = YAML::LoadFile(scene_path.string());
				
				guid = GUID(config["AssetFileGUID"].as<uint64_t>());
				scene->name = config["Name"].as<std::string>();
				scene->guid = GUID(config["GUID"].as<uint64_t>());
				scene->assetFileGUID = guid;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
			}

			assets.push_back(scene);
			loaded = true;

			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			scene_path = assetPath;
			
			LX_CORE_INFO("Scene Files are not saved regularly");

			return true;
		}

		std::filesystem::path scene_path;
	};
}
