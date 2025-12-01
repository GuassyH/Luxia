#pragma once
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API ModelFile : public AssetFile {
	public:
		ModelFile() = default;
		ModelFile(const std::filesystem::path m_modelPath)
			: modelPath(m_modelPath) {
			type = AssetType::ModelType;
		}
		ModelFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~ModelFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			std::ifstream infile(m_assetPath, std::ios::binary);

			int success = 0;
			std::string line;
			while (std::getline(infile, line)) {
				// Trim whitespace if needed
				if (line.rfind("path=", 0) == 0) {
					modelPath = line.substr(5);
					success++;
				}
			}

			infile.close();
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			std::ofstream outfile(m_assetPath, std::ios::binary);

			outfile << "path=" << modelPath.string() << "\n";

			outfile.close();
			return true;
		}

		std::filesystem::path modelPath;
	};
}
