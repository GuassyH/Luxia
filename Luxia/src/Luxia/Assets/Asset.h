#pragma once

#include "Luxia/Core/Core.h"

#include <filesystem>
#include <iostream>

namespace Luxia::Assets {

	enum LUXIA_API AssetType {
		NoAsset = 0,
		Model = 1,
		Texture = 2,
		Audio = 3,
	};

	class LUXIA_API Asset {
	public:
		Asset() = default;
		virtual ~Asset() = default;

		virtual void Load(const std::filesystem::path& m_path, const std::string& m_name = "no_name") = 0; // Load the asset
		virtual void Unload() = 0; // Unload the asset
		virtual void Delete() = 0;

		AssetType type = AssetType::NoAsset;

		std::string suffix			= ".none"; // .png, .obj, etc. Helps to decide how to load
		std::filesystem::path path  = "no/path";
		std::string name			= "no_name";

		bool loaded = false;
	};

}