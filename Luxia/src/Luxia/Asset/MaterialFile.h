#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia::Assets {
	class LUXIA_API MaterialFile : public AssetFile {
	public:
		MaterialFile() = default;
		MaterialFile(const std::filesystem::path t1) {} // Dummy constructor to avoid template issues
		MaterialFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~MaterialFile() = default;

		GUID shaderGUID = GUID(0);
		GUID diffuseGUID = GUID(0);
		GUID specularGUID = GUID(0);
		GUID normalsGUID = GUID(0);

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::MaterialType;

			std::shared_ptr<Luxia::IMaterial> mat = Platform::Assets::CreateMaterial();
			assets.push_back(mat);

			Save(m_assetPath);
			return true;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::MaterialType;

			assetPath = m_assetPath;
			loaded = false;

			std::shared_ptr<Luxia::IMaterial> mat = Platform::Assets::CreateMaterial();

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				mat->name = config["Name"].as<std::string>();
				mat->guid = GUID(config["GUID"].as<uint64_t>());

				if (auto propConfig = config["Properties"]) {
					shaderGUID = GUID(propConfig["Shader"].as<uint64_t>());
					mat->color = propConfig["Color"].as<glm::vec4>();
					mat->metallic = propConfig["Metallic"].as<float>();
					mat->roughness = propConfig["Roughness"].as<float>();
					diffuseGUID = GUID(propConfig["Diffuse"].as<uint64_t>());
					specularGUID = GUID(propConfig["Specular"].as<uint64_t>());
					normalsGUID = GUID(propConfig["Normals"].as<uint64_t>());
				}

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML MAT error: " << ex.what() << "\n";
				loaded = false;
			}

			assets.push_back(mat);
			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			if (assets.empty()) return false;

			std::shared_ptr<Luxia::IMaterial> mat = std::dynamic_pointer_cast<Luxia::IMaterial>(assets[0]);
			if (!mat) return false;

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << mat->name;
			out << YAML::Key << "GUID" << YAML::Value << (uint64_t)mat->guid;
			out << YAML::Key << "Properties" << YAML::BeginMap;

			out << YAML::Key << "Shader" << YAML::Value << (mat->shader ? (uint64_t)mat->shader->guid : 0);
			out << YAML::Key << "Color" << YAML::Value << mat->color;
			out << YAML::Key << "Metallic" << YAML::Value << mat->metallic;
			out << YAML::Key << "Roughness" << YAML::Value << mat->roughness;
			out << YAML::Key << "Diffuse" << YAML::Value << (mat->diffuse_texture ? (uint64_t)mat->diffuse_texture->guid : 0);
			out << YAML::Key << "Specular" << YAML::Value << (mat->specular_texture ? (uint64_t)mat->specular_texture->guid : 0);
			out << YAML::Key << "Normals" << YAML::Value << (mat->normal_texture ? (uint64_t)mat->normal_texture->guid : 0);

			out << YAML::EndMap;
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
