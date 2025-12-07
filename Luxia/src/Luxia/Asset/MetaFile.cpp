#include "lxpch.h"
#include "MetaFile.h"


namespace Luxia::Assets {
	bool MetaFile::Create(const std::filesystem::path& m_assetPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type) {
		type = m_type;
		assetPath = m_assetPath.lexically_normal();
		metaPath = m_metaPath.lexically_normal();
		name = m_name;

		// Save 
		Save();
		loaded = true;

		return loaded;
	}

	// Should be YAML!
	bool MetaFile::Load(const std::filesystem::path& m_metaPath) {
		// Open file
		metaPath = m_metaPath;

		try {
			YAML::Node config = YAML::LoadFile(metaPath.string());

			type = static_cast<Luxia::AssetType>(config["type"].as<int>());
			guid = Luxia::GUID(config["guid"].as<uint64_t>());
			assetPath = std::filesystem::path(config["assetPath"].as<std::string>());
			metaPath = std::filesystem::path(config["metaPath"].as<std::string>());
			name = config["name"].as<std::string>();
		
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
		out << YAML::Key << "type";
		out << YAML::Value << (int)type;
		out << YAML::Key << "guid";
		out << YAML::Value << (uint64_t)guid;
		out << YAML::Key << "assetPath";
		out << YAML::Value << assetPath.string();
		out << YAML::Key << "metaPath";
		out << YAML::Value << metaPath.string();
		out << YAML::Key << "name";
		out << YAML::Value << name;
		out << YAML::EndMap;

		// Open file
		std::ofstream outfile(metaPath);
		if (!outfile.is_open()) {
			LX_CORE_ERROR("Failed to save metafile: {}", metaPath.string());
			return false;
		}

		outfile << out.c_str();

		outfile.close();

		return true;
	}

	void MetaFile::Unload() {
		loaded = false;
	}
}