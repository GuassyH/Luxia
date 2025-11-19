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
	//	Scene = 6
	};
}

namespace Luxia::Assets {

	class LUXIA_API AssetFile {
	public:
		AssetFile() = default;
		virtual ~AssetFile() = default;

		virtual void Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type); // Create the metafile from a source
		virtual bool Load(const std::filesystem::path& m_metaPath); // Load the metafile
		virtual bool Save();
		virtual void Unload(); // Unload the asset

		AssetType type = AssetType::NoAsset;

		std::string extension = ".none"; // .png, .obj, etc. Helps to decide how to load
		std::filesystem::path srcPath = "no/path";
		std::filesystem::path metaPath = "no/path";
		std::string name= "no_name";

		bool loaded = false;

		GUID guid;
	protected:
	};

}