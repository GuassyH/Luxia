#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"
#include "Luxia/Core/Log.h"

#include <filesystem>
#include <iostream>

namespace Luxia {
	enum LUXIA_API AssetType {
		NoAsset = 0,
		Model = 1,
		Texture = 2,
		Material = 3,
		Shader = 4,
		Audio = 5,
	};
}

namespace Luxia::Assets {

	class LUXIA_API AssetFile {
	public:
		AssetFile() = default;
		virtual ~AssetFile() = default;

		virtual void Create(const std::filesystem::path& sourcePath) = 0; // Create the metafile from a source
		virtual void Load(const std::filesystem::path& metaPath) = 0; // Load the metafile
		virtual void Unload() = 0; // Unload the asset

		uint64_t GetGUID() const { return guid.Get(); }

		AssetType type = AssetType::NoAsset;

		std::string suffix			= ".none"; // .png, .obj, etc. Helps to decide how to load
		std::filesystem::path srcPath = "no/path";
		std::string name			= "no_name";

		std::string metadata;

		bool loaded = false;
	protected:
		GUID guid;
	};

}