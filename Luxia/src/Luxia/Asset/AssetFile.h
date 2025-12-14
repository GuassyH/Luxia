#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Core/GUID.h"
#include "luxia/Asset/MetaFile.h"
#include "Luxia/CustomYAML.h"

#include "Asset.h"

#include <filesystem>
#include <iostream>
#include <vector>


namespace Luxia::Assets {

	class LUXIA_API AssetFile {
	public:
		AssetFile() = default;
		virtual ~AssetFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) { return false; } 
		virtual std::vector<std::shared_ptr<Asset>> Load(const std::filesystem::path& m_assetPath)
		{ return assets; }
		virtual bool Save(const std::filesystem::path& m_assetPath)	{ return false; }
		virtual void Unload() {} // Unload the asset

		std::vector<std::shared_ptr<Asset>> Load()
		{ if (!std::filesystem::exists(assetPath)) { LX_CORE_ERROR("assetPath not set"); return assets; } return Load(assetPath); }
		bool Save() { if (!std::filesystem::exists(assetPath)) { LX_CORE_ERROR("assetPath not set"); return false; } return Save(assetPath); }


		bool loaded = false;

		GUID guid;
		std::vector<std::shared_ptr<Asset>> assets = std::vector<std::shared_ptr<Asset>>(0);
		AssetType type = AssetType::NoType;
		std::filesystem::path assetPath = "no/path";
	};

}