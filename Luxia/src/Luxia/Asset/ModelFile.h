#pragma once
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API ModelFile : public AssetFile {
	public:
		ModelFile() = default;
		~ModelFile() = default;

		virtual bool LoadExtra(const std::filesystem::path& m_metaPath) {
			return true;
		}
		virtual bool SaveExtra(const std::filesystem::path& m_metaPath) {
			return true;
		}
	};
}
