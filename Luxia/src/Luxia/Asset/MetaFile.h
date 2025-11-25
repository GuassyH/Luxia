#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API MetaFile
	{
		MetaFile() = default;
		~MetaFile() = default;

		bool Create(const std::filesystem::path& m_srcPath, const std::filesystem::path& m_relativePath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type); // Create the metafile from a source
		bool Load(const std::filesystem::path& m_metaPath);
		bool Save();
		void Unload(); // Unload the asset

		AssetType type = AssetType::NoType;

		std::string extension = ".none"; // .png, .obj, etc. Helps to decide how to load
		std::filesystem::path srcPath = "no/path";
		std::filesystem::path relPath = "no/path";
		std::filesystem::path metaPath = "no/path";
		std::string name = "no_name";


		bool loaded = false;

		GUID guid;
	};
}
