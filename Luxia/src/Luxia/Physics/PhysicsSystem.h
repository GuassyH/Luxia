#pragma once

#include "Luxia/Core/Core.h"
#include "PhysicsWorld.h"

#include "Luxia/Core/Log.h"

#include "Jolt/Jolt.h"
#include "Jolt/Core/Factory.h"
#include "Jolt/RegisterTypes.h"
#include "Jolt/Core/JobSystemThreadPool.h"
#include "Jolt/Core/TempAllocator.h"

JPH_SUPPRESS_WARNINGS_STD_BEGIN
#include <iostream>
JPH_SUPPRESS_WARNINGS_STD_END


namespace Luxia::Physics {

	class PhysicsSystem {
	public:
		static void Initialize();
		static void Shutdown();

		// static JPH::JobSystemThreadPool* sJobSystem;
		// static JPH::TempAllocatorImpl* sTempAllocator;

		// Physics world
		static std::unique_ptr<PhysicsWorld> CreateWorld(const PhysicsWorldDesc& physicsWorldDesc);
		static void DeleteWorld(PhysicsWorld* world);

	protected:


	};

}