#pragma once

#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API ShaderFile : public AssetFile {
	public:
		ShaderFile() = default;
		~ShaderFile() = default;
		ShaderFile(const std::filesystem::path& frag_path, const std::filesystem::path& vert_path) 
			: fragPath(frag_path), vertPath(vert_path) { }

		virtual bool LoadExtra(const std::filesystem::path& m_metaPath) {
			std::ifstream infile(m_metaPath, std::ios::in);

			int success = 0;
			std::string line;
			while (std::getline(infile, line)) {
				// Trim whitespace if needed
				if (line.rfind("frag=", 0) == 0) {
					fragPath = line.substr(5);
					success++;
				}
				else if (line.rfind("vert=", 0) == 0) {
					vertPath = line.substr(5);
					success++;
				}
			}


			if (success != 2) { return false; }

			infile.close();

			return true;

		}
		virtual bool SaveExtra(const std::filesystem::path& m_metaPath) {
			// Temporary solution
			// This copies the metadata in to the file


			std::string full;
			std::string line;

			// Whyyyyyyyyyy
			std::ifstream infile(m_metaPath, std::ios::in);

			while (std::getline(infile, line)) {
				// Trim whitespace if needed
				full += line + "\n";
			}

			infile.close();

			std::ofstream outfile(m_metaPath);

			outfile << full;
			outfile << "Shader:\n";
			outfile << "frag=" << fragPath.string() << "\n";
			outfile << "vert=" << vertPath.string() << "\n";

			outfile.close();

			return true;
		}

		std::filesystem::path fragPath;
		std::filesystem::path vertPath;
	};
}