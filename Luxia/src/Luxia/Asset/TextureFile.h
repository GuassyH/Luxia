#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia::Assets {
	class LUXIA_API TextureFile : public AssetFile {
	public:
		TextureFile() = default;
		TextureFile(const std::filesystem::path m_texturePath)
			: texture_path(m_texturePath.lexically_normal()) {
		}
		TextureFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~TextureFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::TextureType;
			guid = GUID();

			std::shared_ptr<Luxia::ITexture> texture = Platform::Assets::CreateTexture();
			texture->name = m_assetPath.filename().replace_extension("").string();
			texture->LoadFromFile(texture_path);
			texture->assetFileGUID = guid;
			texture->guid = GUID();

			assets.push_back(texture);

			Save(m_assetPath);
			return true;
		}


		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			type = Luxia::AssetType::TextureType;

			assetPath = m_assetPath.lexically_normal();
			loaded = false;
			// Only save if the assetfile path already exists??

			std::shared_ptr<Luxia::ITexture> texture = Platform::Assets::CreateTexture();

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());
				guid = GUID(config["AssetFileGUID"].as<uint64_t>());

				// Check if missing
				texture->name = config["Name"].as<std::string>();
				texture->guid = GUID(config["GUID"].as<uint64_t>());
				texture->LoadFromFile(config["Path"].as<std::string>(), config["Properties"]["Flip"].as<bool>());
				texture_path = config["Path"].as<std::string>();
				texture->assetFileGUID = guid;

				auto propConfig = config["Properties"];
				
				// Set num colch etc

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			assets.push_back(texture);

			return assets;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;

			if (assets.empty()) return false;
			std::shared_ptr<Luxia::ITexture> texture = std::dynamic_pointer_cast<Luxia::ITexture>(assets[0]);
			if (!texture) return false;

			YAML::Emitter out;

			out << YAML::BeginMap;

			out << YAML::Key << "AssetFileGUID" << YAML::Value << (uint64_t)guid;
			out << YAML::Key << "Name" << YAML::Value << texture->name;
			out << YAML::Key << "GUID" << YAML::Value << texture->guid;
			out << YAML::Key << "Path" << YAML::Value << texture->path.string();
			out << YAML::Key << "Properties" << YAML::BeginMap;

			out << YAML::Key << "Flip" << YAML::Value << texture->GetFlip();
			out << YAML::Key << "Width" << YAML::Value << texture->GetWidth();
			out << YAML::Key << "Height" << YAML::Value << texture->GetHeight();
			out << YAML::Key << "NumColCh" << YAML::Value << texture->GetNumColCh();

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

		std::filesystem::path texture_path;
	};
}
