#pragma once

#include "Luxia/Core/Core.h"
#include "entt/entt.hpp"

namespace Luxia {
	class LUXIA_API Scene
	{
	public:
		Scene() : registry(entt::registry()) {}
		~Scene() = default;

		entt::registry registry;
	private:
		// guid
	};
}

