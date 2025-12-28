#pragma once

#include <filesystem>
#include "Luxia/Core/Core.h"

namespace Luxia {
	const enum class LUXIA_API AssetType {
		NoType			= 0,
		ModelType		= 1,
		TextureType		= 2,
		MaterialType	= 3,
		ShaderType		= 4,
		AudioType		= 5,
		SceneType		= 6,
		MeshType		= 7,
	};
	inline AssetType PeakAssetType(const std::filesystem::path& metafile_path) {
		if (!std::filesystem::exists(metafile_path)) { return Luxia::AssetType::NoType; }

		std::ifstream infile(metafile_path);
		if (!infile.is_open()) { return Luxia::AssetType::NoType; }

		std::string line;
		Luxia::AssetType type = Luxia::AssetType::NoType;
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