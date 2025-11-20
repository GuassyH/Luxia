#include "lxpch.h"
#include "AssetFile.h"


namespace Luxia::Assets {
	bool AssetFile::Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type) {
		type = m_type;
		srcPath = m_srcPath;
		metaPath = m_metaPath;
		extension = m_srcPath.extension().string();
		name = m_name;
		
		// Save 
		loaded = Save();

		return loaded;
	}
	bool AssetFile::Load(const std::filesystem::path& m_metaPath){
		// Open file
		metaPath = m_metaPath;
		std::ifstream infile(metaPath);

		// Throw runtime error
		if (!infile.is_open()) {
			LX_CORE_ERROR("Failed to open metafile: {}", metaPath.string());
			return false;
		}

		// temp
		int success = 0;
		std::string line;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("type=", 0) == 0) {            // starts with "guid="
				uint64_t val = std::stoull(line.substr(5));
				type = static_cast<Luxia::AssetType>(val);
				success++;
			}
			else if (line.rfind("guid=", 0) == 0) {            // starts with "guid="
				guid = std::stoull(line.substr(5));
				success++;
			}
			else if (line.rfind("path=", 0) == 0) {       // starts with "src="
				srcPath = line.substr(5);
				success++;
			}
			else if (line.rfind("name=", 0) == 0) {            // starts with "guid="
				name = line.substr(5);
				success++;
			}
			else if (line.rfind("extn=", 0) == 0) {            // starts with "guid="
				extension = line.substr(5);
				success++;
			}
		}

		// if all lines werent found, return false
		if (success != 5) { return false; }

		return true;
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
		if (!outfile.is_open()) {
			LX_CORE_ERROR("Failed to save metafile: {}", metaPath.string());
			return false;
		}

		// outfile << out.c_str();
		outfile << "type=" << (int)type					<< "\n";
		outfile << "guid=" << (uint64_t)guid			<< "\n";
		outfile << "path=" << srcPath.string().c_str()	<< "\n";
		outfile << "name=" << name.c_str() << "\n";
		outfile << "extn=" << extension.c_str()				<< "\n";

		outfile.close();
		return true;
	}

	void AssetFile::Unload(){

	}
}