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
	AssetType PeakAssetType(const std::filesystem::path& metafile_path);
}

namespace Luxia::Assets {

	class LUXIA_API AssetFile {
	public:
		AssetFile() = default;
		virtual ~AssetFile() = default;

		virtual bool Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_relativePath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type); // Create the metafile from a source
		bool Load(const std::filesystem::path& m_metaPath); 
		bool Save();
		virtual void Unload(); // Unload the asset

		virtual bool LoadExtra(const std::filesystem::path& m_metaPath) { return true; }
		virtual bool SaveExtra(const std::filesystem::path& m_metaPath) { return true; }

		AssetType type = AssetType::NoAsset;

		std::string extension = ".none"; // .png, .obj, etc. Helps to decide how to load
		std::filesystem::path srcPath = "no/path";
		std::filesystem::path relPath = "no/path";
		std::filesystem::path metaPath = "no/path";
		std::string name= "no_name";


		bool loaded = false;

		GUID guid;
	protected:
	};

}