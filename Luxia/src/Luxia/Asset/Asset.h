#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"

namespace Luxia::Assets {
	class LUXIA_API Asset {
	public:
		GUID guid;

		Asset() = default;
		virtual ~Asset() = default;
	};
}
