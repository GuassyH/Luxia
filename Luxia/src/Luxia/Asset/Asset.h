#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"

namespace Luxia::Assets {
	class LUXIA_API Asset {
	public:
		GUID guid;
		std::string name;

		// Asset() = default;
		Asset() : guid(GUID()), name("no_name") {}
		virtual ~Asset() = default;
		virtual bool Unload() = 0;
	};
}
