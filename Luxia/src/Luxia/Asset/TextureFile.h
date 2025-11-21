#pragma once
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API TextureFile : public AssetFile {
	public:
		TextureFile() = default;
		~TextureFile() = default;

		virtual bool LoadExtra(const std::filesystem::path& m_metaPath) {
			return true;
		}
		virtual bool SaveExtra(const std::filesystem::path& m_metaPath) {
			return true;
		}
	};
}
