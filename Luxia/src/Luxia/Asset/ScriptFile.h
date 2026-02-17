#pragma once

#include "Luxia/Asset/AssetFile.h"


#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia::Assets {
	class LUXIA_API ScriptFile : public AssetFile {
	public:
		ScriptFile() = default;
		ScriptFile(const std::filesystem::path m_scriptPath)
			: script_path(m_scriptPath.lexically_normal()) {
		}
		ScriptFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~ScriptFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::Script;
			guid = GUID();

			std::ofstream outfile(m_assetPath); // text mode

			Save(m_assetPath);
			return true;
		}


		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::Script;

			// Load script

			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			if (assets.empty()) return false;
			// std::shared_ptr<Luxia::IScript> script = std::dynamic_pointer_cast<Luxia::IScript>(assets[0]);
			// if (!script) return false;

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "AssetFileGUID" << YAML::Value << (uint64_t)guid;
			// out << YAML::Key << "GUID" << YAML::Value << (uint64_t)guid;

			out << YAML::EndMap;


			if (!out.good()) {
				LX_CORE_ERROR("Emitter failed!");
				return false;
			}


			// Write to file
			std::ofstream outfile(assetPath); // text mode
			if (!outfile.is_open()) {
				LX_CORE_ERROR("Failed to save scriptfile: {}", assetPath.string());
				return false;
			}

			outfile << out.c_str(); // write YAML text

			outfile.close();

			return true;
		}

		std::filesystem::path script_path;
	};
}
