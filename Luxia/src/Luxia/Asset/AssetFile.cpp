#include "lxpch.h"
#include "AssetFile.h"

namespace Luxia {
	AssetType PeakAssetType(const std::filesystem::path& metafile_path) {
		if (!std::filesystem::exists(metafile_path)) { return Luxia::AssetType::NoType; }

		std::ifstream infile(metafile_path);
		if (!infile.is_open()) { return Luxia::AssetType::NoType; }

		std::string line;
		Luxia::AssetType type = Luxia::AssetType::NoType;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("type=", 0) == 0) {
				uint64_t val = std::stoull(line.substr(5));
				type = static_cast<Luxia::AssetType>(val);
			}
		}
		infile.close();
		return type;
	}
}

namespace Luxia::Assets { 
	bool AssetFile::Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_relativePath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type) {
		type = m_type;
		srcPath = m_srcPath.lexically_normal();
		relPath = m_relativePath.lexically_normal();
		metaPath = m_metaPath.lexically_normal();
		extension = m_srcPath.extension().string();
		name = m_name;
		
		// Save 
		Save();
		loaded = true;

		return loaded;
	}

	// Should be YAML!
	bool AssetFile::Load(const std::filesystem::path& m_metaPath){
		// Open file
		metaPath = m_metaPath;
		std::ifstream infile(metaPath, std::ios::in);

		// Throw runtime error
		if (!infile.is_open()) {
			LX_CORE_ERROR("Failed to open metafile: {}", metaPath.string());
			return false;
		}

		int success = 0;
		std::string line;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("type=", 0) == 0) {        
				uint64_t val = std::stoull(line.substr(5));
				type = static_cast<Luxia::AssetType>(val);
				success++;
			}
			else if (line.rfind("guid=", 0) == 0) {        
				guid = std::stoull(line.substr(5));
				success++;
			}
			else if (line.rfind("path=", 0) == 0) {   
				srcPath = line.substr(5);
				success++;
			}
			else if (line.rfind("rel_path=", 0) == 0) {   
				relPath = line.substr(9);
				success++;
			}
			else if (line.rfind("name=", 0) == 0) {
				name = line.substr(5);
				success++;
			}
			else if (line.rfind("extn=", 0) == 0) {
				extension = line.substr(5);
				success++;
			}
		}
		
		infile.close();

		// if all lines werent found, return false
		if (success != 6) { return false; }
		
		return LoadExtra(metaPath);
	}

	// Should be YAML!
	bool AssetFile::Save() {
		// Open file
		std::ofstream outfile(metaPath);
		if (!outfile.is_open()) {
			LX_CORE_ERROR("Failed to save metafile: {}", metaPath.string());
			return false;
		}

		// outfile << out.c_str();
		outfile << "type=" << (int)type				<< "\n";
		outfile << "guid=" << (uint64_t)guid		<< "\n";
		outfile << "path=" << srcPath.string()		<< "\n";
		outfile << "rel_path=" << relPath.string()	<< "\n";
		outfile << "name=" << name					<< "\n";
		outfile << "extn=" << extension				<< "\n";

		outfile.close();

		return SaveExtra(metaPath);
	}

	void AssetFile::Unload(){

	}
}