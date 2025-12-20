#include "lxpch.h"
#include "MetaFile.h"


namespace Luxia::Assets {
	bool MetaFile::Create(const std::filesystem::path& m_assetPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type) {
		type = m_type;
		assetPath = m_assetPath.lexically_normal();
		metaPath = m_metaPath.lexically_normal();

		guid = Luxia::GUID();
		// Save 
		Save();
		loaded = true;

		return loaded;
	}

	// Should be YAML!
	bool MetaFile::Load(const std::filesystem::path& m_metaPath) {
		// Open file
		metaPath = m_metaPath;
		loaded = false;

		try {
			YAML::Node config = YAML::LoadFile(metaPath.string());

			if (!config["Type"] || !config["GUID"] || !config["AssetPath"] || !config["MetaPath"]) {
				std::cerr << "YAML missing required fields in: " << metaPath << "\n";
				return false;
			}

			type = static_cast<Luxia::AssetType>(config["Type"].as<int>());
			guid = Luxia::GUID(config["GUID"].as<uint64_t>());
			assetPath = std::filesystem::path(config["AssetPath"].as<std::string>());
			metaPath = std::filesystem::path(config["MetaPath"].as<std::string>());
		
			loaded = true;
		}
		catch (const YAML::Exception& ex) {
			std::cerr << "YAML error: " << ex.what() << "\n";
			loaded = false;
		} 

		return loaded;
	}

	// Should be YAML!
	bool MetaFile::Save() {
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << (int)type;
		out << YAML::Key << "GUID" << YAML::Value << (uint64_t)guid;
		out << YAML::Key << "AssetPath" << YAML::Value << assetPath.string();
		out << YAML::Key << "MetaPath" << YAML::Value << metaPath.string();
		out << YAML::EndMap;

		if (!out.good()) {
			LX_CORE_ERROR("Emitter failed!");
			return false;
		}

		// Write to file
		std::ofstream outfile(metaPath); // text mode
		if (!outfile.is_open()) {
			LX_CORE_ERROR("Failed to save metafile: {}", metaPath.string());
			return false;
		}

		outfile << out.c_str(); // write YAML text

		outfile.close();

		return true;
	}

	void MetaFile::Unload() {
		loaded = false;
	}
}