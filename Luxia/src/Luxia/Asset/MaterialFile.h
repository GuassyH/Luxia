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

		virtual bool Create(const std::filesystem::path& m_assetPath) override {
			Save(m_assetPath);
			return true;
		}

		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			loaded = false;

			std::shared_ptr<Asset> mat = Platform::Assets::CreateMaterial();

			try {
				YAML::Node config = YAML::LoadFile(assetPath.string());

				loaded = true;
			}
			catch (const YAML::Exception& ex) {
				std::cerr << "YAML error: " << ex.what() << "\n";
				loaded = false;
			}

			assets.push_back(mat);
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

	};
}
