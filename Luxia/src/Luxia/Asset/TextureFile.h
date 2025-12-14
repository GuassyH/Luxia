#pragma once
#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia::Assets {
	class LUXIA_API TextureFile : public AssetFile {
	public:
		TextureFile() = default;
		TextureFile(const std::filesystem::path m_texturePath)
			: texture_path(m_texturePath) {
			type = AssetType::TextureType;
		}
		TextureFile(const std::filesystem::path t1, const std::filesystem::path t2) {} // Dummy constructor to avoid template issues
		~TextureFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}


		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			std::shared_ptr<Asset> texture = Platform::Assets::CreateTexture();

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				// Check if missing
				
				
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

			YAML::Emitter out;

			out << YAML::BeginMap;

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
