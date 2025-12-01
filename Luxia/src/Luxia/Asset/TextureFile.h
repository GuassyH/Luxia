#pragma once
#include "Luxia/Asset/AssetFile.h"

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

		virtual bool Load(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			return true;
		}
		virtual bool Save(const std::filesystem::path& m_assetPath) override {
			assetPath = m_assetPath;
			return true;
		}

		std::filesystem::path texture_path;
	};
}
