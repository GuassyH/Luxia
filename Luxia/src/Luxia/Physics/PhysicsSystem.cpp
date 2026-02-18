#include "lxpch.h"
#include "PhysicsSystem.h"
#include "PhysicsWorld.h"

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>

namespace Luxia::Physics {



	JPH::JobSystemThreadPool* PhysicsSystem::sJobSystem = nullptr;
	JPH::TempAllocatorImpl* PhysicsSystem::sTempAllocator = nullptr;

	void JoltTraceImpl(const char* inFMT, ...) {
		va_list args;
		va_start(args, inFMT);

		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, args);
		va_end(args);

		LX_CORE_TRACE("[Physics] {}", buffer);
	}

	bool JoltAssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine) {
		LX_CORE_ERROR("[Physics]:\nExpression: {}\nMessage: {}\nFile: {}:{}", inExpression, (inMessage ? inMessage : "None"), inFile, inLine);
		return true;
	}


	// set the funcs
	void PhysicsSystem::Initialize() {
		// Set the default logging stuff for JPH
		JPH::RegisterDefaultAllocator();

		JPH::Trace = JoltTraceImpl;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = JoltAssertFailedImpl;)

		// Create factory, responsible for creating instances
		JPH::Factory::sInstance = new JPH::Factory();

		// Register all physics types
		JPH::RegisterTypes();

		// Set temp allocator (10mb)
		sTempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
		
		// Create the job system (multithreaded)
		JPH::uint numThreads = std::thread::hardware_concurrency() - 1;
		static constexpr JPH::uint maxJobs = 2048;
		static constexpr JPH::uint maxBarriers = 16;
		sJobSystem = new JPH::JobSystemThreadPool(maxJobs, maxBarriers, numThreads);
	}


	void PhysicsSystem::Shutdown() {
		LX_CORE_INFO("[Physics]: Shutdown");

		delete sJobSystem;
		sJobSystem = nullptr;

		delete sTempAllocator;
		sTempAllocator = nullptr;

		JPH::UnregisterTypes();

		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;

		LX_CORE_INFO("[Physics]: Shutdown");
	}

	std::unique_ptr<PhysicsWorld> PhysicsSystem::CreateWorld(const PhysicsWorldDesc& physicsWorldDesc) {
		std::unique_ptr<PhysicsWorld> world = std::make_unique<PhysicsWorld>();
		world->Initialize(physicsWorldDesc);
		return world;
	}
	void PhysicsSystem::DeleteWorld(PhysicsWorld* world) {
		if (!world) return;

		if (world->jphSystem.GetNumBodies() > 0) {
			JPH::BodyIDVector bodies;
			world->jphSystem.GetBodies(bodies);

			auto& body_interface = world->jphSystem.GetBodyInterface();

			for (auto& bodyID : bodies) {
				if (body_interface.IsAdded(bodyID))
					body_interface.RemoveBody(bodyID);
				
				world->jphSystem.GetBodyInterface().DestroyBody(bodyID);
			}

		}

	}
}