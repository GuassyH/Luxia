#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"
#include "Luxia/Asset/AssetType.h"

#include "Luxia/Core/WeakPtrProxy.h"

namespace Luxia::Assets {
	class LUXIA_API Asset {
	public:
		GUID guid = GUID(0);
		GUID assetFileGUID = GUID(0);
		std::string name = "no_name";
		Luxia::AssetType type = Luxia::AssetType::None;

		// Asset() = default;
		Asset() = default;
		virtual ~Asset() = default;
		virtual bool Unload() = 0;
	};
}
