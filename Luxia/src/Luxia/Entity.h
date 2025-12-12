#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"
#include "Luxia/Components/Transform.h"

namespace Luxia{
	class LUXIA_API Entity {
	public:
		Luxia::GUID guid;
		std::string name;

		Entity() : guid(GUID()), name("Entity") {}

		Luxia::Components::Transform* transform = nullptr;
	};

}