#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

 
#include "Jolt/Jolt.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Core/TempAllocator.h"

#include <iostream>

namespace Luxia::Physics {
	class PhysicsWorld;
	struct PhysicsWorldDesc;
	
	class LUXIA_API PhysicsSystem {
	public:
		static void Initialize();
		static void Shutdown();

		static JPH::JobSystemThreadPool* sJobSystem;
		static JPH::TempAllocatorImpl* sTempAllocator;

		// Physics world
		static std::unique_ptr<PhysicsWorld> CreateWorld(const PhysicsWorldDesc& physicsWorldDesc);
		static void DeleteWorld(PhysicsWorld* world);
	};

}