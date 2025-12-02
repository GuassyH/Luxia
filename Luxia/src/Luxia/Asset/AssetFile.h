#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Core/GUID.h"
#include "luxia/Asset/MetaFile.h"

#include <filesystem>
#include <iostream>

namespace Luxia::Assets {

	class LUXIA_API AssetFile {
	public:
		AssetFile() = default;
		virtual ~AssetFile() = default;

		virtual bool Create(const std::filesystem::path& m_assetPath) { return false; } 
		virtual bool Load(const std::filesystem::path& m_assetPath)	{ return false; }
		virtual bool Save(const std::filesystem::path& m_assetPath)	{ return false; }
		virtual void Unload() {} // Unload the asset

		bool Load() { if (!std::filesystem::exists(assetPath)) { LX_CORE_ERROR("assetPath not set"); return false; } return Load(assetPath); }
		bool Save() { if (!std::filesystem::exists(assetPath)) { LX_CORE_ERROR("assetPath not set"); return false; } return Save(assetPath); }

		bool loaded = false;

		GUID guid;
		AssetType type = AssetType::NoType;
		std::filesystem::path assetPath = "no/path";
	};

}