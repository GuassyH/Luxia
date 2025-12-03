#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "glm/glm.hpp"

namespace Luxia::Assets {
	class LUXIA_API MaterialFile : public AssetFile {
	public:
		MaterialFile() = default;
		MaterialFile(const std::filesystem::path t1) {} // Dummy constructor to avoid template issues
		MaterialFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~MaterialFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
		
			std::ifstream infile(m_assetPath, std::ios::in);

			int success = 0;
			std::string line;
			while (std::getline(infile, line)) {
				// Trim whitespace if needed
				if (line.rfind("shader=", 0) == 0) {
					shader_path = line.substr(7);
					success++;
				}
				if (line.rfind("diff_guid=", 0) == 0) {
					diffuse_guid = GUID(std::stoull(line.substr(10)));
					success++;
				}
				if (line.rfind("spec_guid=", 0) == 0) {
					specular_guid = GUID(std::stoull(line.substr(10)));
					success++;
				}
				if (line.rfind("norm_guid=", 0) == 0) {
					normal_guid = GUID(std::stoull(line.substr(10)));
					success++;
				}
				if (line.rfind("color=", 0) == 0) {
					std::string color_str = line.substr(6);
					sscanf(color_str.c_str(), "%f,%f,%f,%f", &color.r, &color.g, &color.b, &color.a);
					success++;
				}
				if (line.rfind("roughness=", 0) == 0) {
					roughness = std::stof(line.substr(10));
					success++;
				}
				if (line.rfind("metallic=", 0) == 0) {
					metallic = std::stof(line.substr(9));
					success++;
				}
			}

			infile.close();

			if (success != 7) { return false; }

			std::ifstream guid_infile(shader_path, std::ios::in);
			while (std::getline(guid_infile, line)) {
				// Trim whitespace if needed
				if (line.rfind("guid=", 0) == 0) {
					shader_guid = std::stoull(line.substr(5));
					break;
				}
			}

			guid_infile.close();
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			std::ofstream outfile(m_assetPath);

			outfile << "shader=" << shader_path.lexically_normal().string() << "\n";
			outfile << "diff_guid=" << (uint64_t)diffuse_guid << "\n";
			outfile << "spec_guid=" << (uint64_t)specular_guid << "\n";
			outfile << "norm_guid=" << (uint64_t)normal_guid << "\n";
			outfile << "color=" << color.r << "," << color.g << "," << color.b << "," << color.a << "\n";
			outfile << "roughness=" << roughness << "\n";
			outfile << "metallic=" << metallic << "\n";

			outfile.close();

			return true;
		}

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
	};
}
