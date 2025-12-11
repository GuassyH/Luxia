#pragma once
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API SceneFile : public AssetFile {
	public:
		SceneFile() = default;
		SceneFile(const std::filesystem::path m_scenePath)
			: scene_path(m_scenePath) {
			type = AssetType::SceneType;
		}
		SceneFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~SceneFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				// Check if missing


				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			return loaded;
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			YAML::Emitter out;

			out << YAML::BeginMap;
			out << YAML::EndMap;

			if (!out.good()) {
				LX_CORE_ERROR("Emitter failed!");
				return false;
			}


			// Write to file
			std::ofstream outfile(assetPath); // text mode
			if (!outfile.is_open()) {
				LX_CORE_ERROR("Failed to save metafile: {}", assetPath.string());
				return false;
			}

			outfile << out.c_str(); // write YAML text

			outfile.close();

			return true;
		}

		std::filesystem::path scene_path;
	};
}
