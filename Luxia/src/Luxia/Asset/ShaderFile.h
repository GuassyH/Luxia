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
			std::ifstream infile(m_metaPath);

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

			// LX_CORE_ERROR("f:{} \nv:{}", fragPath.string(), vertPath.string());

			if (success != 2) { return false; }
			// LX_CORE_ERROR("LOADED SHADER, type {}", (int)type);

			infile.close();

			return true;

		}
		virtual bool SaveExtra(const std::filesystem::path& m_metaPath) {
			std::ofstream outfile(m_metaPath, std::ios::out | std::ios::app);

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