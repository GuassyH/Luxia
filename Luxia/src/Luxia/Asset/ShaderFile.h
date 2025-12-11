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

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				// Check if missing
				shaderType = static_cast<ShaderFileType>(config["shader_type"].as<int>());

				if (shaderType == ShaderFileType::VertexFragment) {
					fragPath = config["frag_path"].as<std::string>();
					vertPath = config["vert_path"].as<std::string>();
				}
				else {
					shaderPath = config["shader_path"].as<std::string>();
				}

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			return loaded;
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "shader_type";
			out << YAML::Value << (int)shaderType;

			if (shaderType == ShaderFileType::VertexFragment) {
				out << YAML::Key << "frag_path";
				out << YAML::Value << fragPath.string();

				out << YAML::Key << "vert_path";
				out << YAML::Value << vertPath.string();
			}
			else {
				out << YAML::Key << "shader_path";
				out << YAML::Value << shaderPath.string();
			}

			out << YAML::EndMap;

			if (!out.good()) {
				LX_CORE_ERROR("Emitter failed!");
				return false;
			}


			// Write to file
			std::ofstream outfile(assetPath); // text mode
			if (!outfile.is_open()) {
				LX_CORE_ERROR("Failed to save metafile: {}", assetPath.string());
				return false;
			}

			outfile << out.c_str(); // write YAML text

			outfile.close();

			return true;
		}


	};
}