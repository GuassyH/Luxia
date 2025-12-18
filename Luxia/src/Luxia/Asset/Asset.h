#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"
#include "Luxia/Asset/AssetType.h"

namespace Luxia::Assets {
	class LUXIA_API Asset {
	public:
		GUID guid = GUID(0);
		std::string name;
		Luxia::AssetType type;

		// Asset() = default;
		Asset() : guid(GUID()), name("no_name"), type(Luxia::AssetType::NoType) {}
		virtual ~Asset() = default;
		virtual bool Unload() = 0;
	};
}
