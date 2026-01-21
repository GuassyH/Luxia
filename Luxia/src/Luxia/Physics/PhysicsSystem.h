#pragma once

#include "Luxia/Core/Core.h"
#include "PhysicsWorld.h"

namespace Luxia::Physics {

	class PhysicsSystem {
	public:
		static void Initialize();
		static void Shutdown();

		// Physics world
		static PhysicsWorld* CreateWorld(const PhysicsWorldDesc& physicsWorldDesc);
		static void DeleteWorld(PhysicsWorld* world);
	};

}