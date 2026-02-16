#pragma once

#include <filesystem>
#include "Luxia/Core/Core.h"

namespace Luxia {
	const enum class LUXIA_API AssetType {
		None			= 0,
		Model		= 1,
		Texture		= 2,
		Material	= 3,
		Shader		= 4,
		Audio		= 5,
		Scene		= 6,
		Mesh		= 7,
		Script		= 8
	};
	inline AssetType PeakAssetType(const std::filesystem::path& metafile_path) {
		if (!std::filesystem::exists(metafile_path)) { return Luxia::AssetType::None; }

		std::ifstream infile(metafile_path);
		if (!infile.is_open()) { return Luxia::AssetType::None; }

		std::string line;
		Luxia::AssetType type = Luxia::AssetType::None;
		while (std::getline(infile, line)) {
			// Trim whitespace if needed
			if (line.rfind("type=", 0) == 0) {
				uint64_t val = std::stoull(line.substr(5));
				type = static_cast<Luxia::AssetType>(val);
			}
		}
		infile.close();
		return type;
	}
}