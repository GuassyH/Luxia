#pragma once

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"

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
		std::shared_ptr<Luxia::IShader> shader = nullptr;

		// In case its vert and frag
		std::filesystem::path fragPath;
		std::filesystem::path vertPath;

		// In case its something else, eg compute shader
		std::filesystem::path shaderPath;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			shader = Platform::Assets::CreateShader(fragPath.string().c_str(), vertPath.string().c_str());
			assets.push_back(shader);

			Save(m_assetPath);
			return true;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			std::shared_ptr<Asset> shader = nullptr;

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				shaderType = static_cast<ShaderFileType>(config["ShaderType"].as<int>());

				if (shaderType == ShaderFileType::VertexFragment) {
					fragPath = config["FragPath"].as<std::string>();
					vertPath = config["VertPath"].as<std::string>();
					shader = Platform::Assets::CreateShader(fragPath.string().c_str(), vertPath.string().c_str());
					assets.push_back(shader);
					
					shader->guid = GUID(config["GUID"].as<uint64_t>());
					shader->name = config["Name"].as<std::string>();
				}
				else {
					shaderPath = config["ShaderPath"].as<std::string>();
				}

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}


			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "ShaderType" << YAML::Value << (int)shaderType;
			out << YAML::Key << "GUID" << YAML::Value << (uint64_t)shader->guid;
			out << YAML::Key << "Name" << YAML::Value << shader->name;

			if (shaderType == ShaderFileType::VertexFragment) {
				out << YAML::Key << "FragPath" << YAML::Value << fragPath.string().c_str();
				out << YAML::Key << "VertPath" << YAML::Value << vertPath.string().c_str();
			}
			else {
				out << YAML::Key << "ShaderPath" << YAML::Value << shaderPath.string().c_str();
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