#pragma once

#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API ShaderFile : public AssetFile {
	public:

		ShaderFile() = default;
		~ShaderFile() = default;
		ShaderFile(const std::filesystem::path& shader_path)
			: shaderPath(shader_path) {
			type = AssetType::ShaderType;
			shaderType = ShaderFileType::Other;
		}
		ShaderFile(const std::filesystem::path& frag_path, const std::filesystem::path& vert_path) 
			: fragPath(frag_path), vertPath(vert_path) {
			type = AssetType::ShaderType;
			shaderType = ShaderFileType::VertexFragment;
		}

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) {
			assetPath = m_assetPath;

			std::ifstream infile(m_assetPath, std::ios::in);

			int wanted = (shaderType == ShaderFileType::Other) ? 1 : 2;
			int success = 0;
			std::string line;
			while (std::getline(infile, line)) {
				// Trim whitespace if needed
				if(shaderType == ShaderFileType::Other) {
					if (line.rfind("shader=", 0) == 0) {
						shaderPath = line.substr(7);
						success++;
					}
				}
				else if (shaderType == ShaderFileType::VertexFragment) {
					if (line.rfind("frag=", 0) == 0) {
						fragPath = line.substr(5);
						success++;
					}
					else if (line.rfind("vert=", 0) == 0) {
						vertPath = line.substr(5);
						success++;
					}
				}
			}

			if (success != wanted) { return false; }

			infile.close();

			return true;

		}
		virtual bool Save(const std::filesystem::path& m_assetPath) {
			assetPath = m_assetPath;

			std::ofstream outfile(m_assetPath);
			outfile << (int)shaderType << "\n";

			if (shaderType == ShaderFileType::Other) {
				outfile << "shader=" << shaderPath.string() << "\n";
			}
			else if (shaderType == ShaderFileType::VertexFragment) {
				outfile << "frag=" << fragPath.string() << "\n";
				outfile << "vert=" << vertPath.string() << "\n";
			}

			outfile.close();

			return true;
		}


		enum class ShaderFileType {
			VertexFragment,
			Compute,
			Other,
			None
		};
		
		ShaderFileType shaderType = ShaderFileType::None;

		// In case its vert and frag
		std::filesystem::path fragPath;
		std::filesystem::path vertPath;

		// In case its something else, eg compute shader
		std::filesystem::path shaderPath;
	
	};
}