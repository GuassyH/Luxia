#pragma once
#define YAML_CPP_STATIC_DEFINE

#include "Luxia/Core/Core.h"
#include "Luxia/Asset/AssetType.h"
#include "Luxia/Core/GUID.h"

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitter.h>

namespace Luxia::Assets {
	class LUXIA_API MetaFile
	{
	public:
		MetaFile() = default;
		~MetaFile() = default;

		bool Create(const std::filesystem::path& m_assetPath, const std::filesystem::path& m_metaPath, const std::string& m_name, const AssetType& m_type); // Create the metafile from a source
		bool Load(const std::filesystem::path& m_metaPath);
		bool Reload() { return Load(metaPath); }
		bool Save();
		void Unload(); // Unload the asset

		AssetType type = AssetType::NoType;

		std::filesystem::path assetPath = "no/path";
		std::filesystem::path metaPath = "no/path";

		bool loaded = false;

		GUID guid = GUID(0);
	};
}
