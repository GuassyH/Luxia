#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "glm/vec4.hpp"

namespace Luxia::Assets {
	class LUXIA_API MaterialFile : public AssetFile {
	public:
		MaterialFile() = default;
		MaterialFile(const std::filesystem::path t1) {} // Dummy constructor to avoid template issues
		MaterialFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~MaterialFile() = default;


		// Shader
		std::filesystem::path shader_path;
		GUID shader_guid = GUID(0);

		// Input Textures
		GUID diffuse_guid = GUID(0);
		GUID specular_guid = GUID(0);
		GUID normal_guid = GUID(0);

		// Material Properties
		glm::vec4 color = glm::vec4(1.0f);
		float roughness = 1.0f;
		float metallic = 1.0f;


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

				shader_guid =	Luxia::GUID(config["shader_guid"].as<uint64_t>());
				diffuse_guid =	Luxia::GUID(config["diff_guid"].as<uint64_t>());
				specular_guid = Luxia::GUID(config["spec_guid"].as<uint64_t>());
				normal_guid =	Luxia::GUID(config["norm_guid"].as<uint64_t>());
				//color =			config["color"].as<glm::vec4>();
				roughness =		config["roughness"].as<float>();
				metallic =		config["metallic"].as<float>();

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
			out << YAML::Key << "shader_guid"	<< YAML::Value << (uint64_t)shader_guid;
			out << YAML::Key << "diff_guid"		<< YAML::Value << (uint64_t)diffuse_guid;
			out << YAML::Key << "spec_guid"		<< YAML::Value << (uint64_t)specular_guid;
			out << YAML::Key << "norm_guid"		<< YAML::Value << (uint64_t)normal_guid;
			out << YAML::Key << "color";
			out << YAML::Value << YAML::BeginSeq << color.r << color.g << color.b << color.a << YAML::EndSeq;
			out << YAML::Key << "roughness"		<< YAML::Value << roughness;
			out << YAML::Key << "metallic"		<< YAML::Value << metallic;
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
