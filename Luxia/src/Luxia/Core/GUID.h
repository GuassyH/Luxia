#pragma once

#include "Luxia/Core/Core.h"

#include <iostream>


namespace Luxia {
	struct GUID {
		uint64_t Get() const { return guid; }
		uint64_t Create() {
			// Create GUID

			return guid;
		}

	private:
		uint64_t guid;
	};
}