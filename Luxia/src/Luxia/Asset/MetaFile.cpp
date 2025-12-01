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
			else if (line.rfind("assetPath=", 0) == 0) {
				assetPath = line.substr(10);
				success++;
			}
			else if (line.rfind("metaPath=", 0) == 0) {
				metaPath = line.substr(9);
				success++;
			}
			else if (line.rfind("name=", 0) == 0) {
				name = line.substr(5);
				success++;
			}
		}

		infile.close();

		// if all lines werent found, return false
		if (success != 5) { return false; }

		loaded = true;
		return loaded;
	}

	// Should be YAML!
	bool MetaFile::Save() {
		// Open file
		std::ofstream outfile(metaPath);
		if (!outfile.is_open()) {
			LX_CORE_ERROR("Failed to save metafile: {}", metaPath.string());
			return false;
		}

		// outfile << out.c_str();
		outfile << "type=" << (int)type << "\n";
		outfile << "guid=" << (uint64_t)guid << "\n";
		outfile << "assetPath=" << assetPath.string() << "\n";
		outfile << "metaPath=" << metaPath.string() << "\n";
		outfile << "name=" << name << "\n";

		outfile.close();

		return true;
	}

	void MetaFile::Unload() {
		loaded = false;
	}
}