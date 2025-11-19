#include "lxpch.h"
#include "AssetFile.h"


namespace Luxia::Assets {
	void AssetFile::Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type) {
		type = m_type;
		srcPath = m_srcPath;
		metaPath = m_metaPath;
		extension = m_srcPath.extension().string();
		name = m_name;
		
		// Save 
		Save();

		loaded = true;
	}
	void AssetFile::Load(const std::filesystem::path& m_metaPath){
		// Open file
		metaPath = m_metaPath;
		std::ifstream infile(metaPath);

		// Throw runtime error
		if (!infile.is_open()) {
			LX_CORE_ERROR("Failed to open metafile: {}", metaPath.string());
		}

		// temp
		std::string line;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("guid=", 0) == 0) {            // starts with "guid="
				guid = std::stoull(line.substr(5));       
			}
			else if (line.rfind("path=", 0) == 0) {       // starts with "src="
				srcPath = line.substr(5);
			}
		}
	}

	bool AssetFile::Save() {
		// Open file
	
		// Savethe data (src, name, guid)

		/*
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Type";
		out << YAML::Value << type;
		out << YAML::Key << "GUID";
		out << YAML::Value << (uint64_t)guid;

		out << YAML::EndMap;
		*/

		std::ofstream outfile(metaPath);
		// outfile << out.c_str();
		outfile << "guid=" << (uint64_t)guid << "\n";
		outfile << "path=" << srcPath.string().c_str();


		outfile.close();

		return true;
	}

	void AssetFile::Unload(){

	}
}