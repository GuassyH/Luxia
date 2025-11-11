#pragma once

#include "Luxia/Core/Core.h"
#include <iostream>
#include <entt/entt.hpp>


namespace Luxia::Components {
	struct Transform;

	struct LUXIA_API Component {
		// data
		Transform* transform;
	};
}