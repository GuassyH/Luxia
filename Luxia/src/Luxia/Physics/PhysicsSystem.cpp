#include "lxpch.h"
#include "PhysicsSystem.h"



namespace Luxia::Physics {



	// JPH::JobSystemThreadPool* PhysicsSystem::sJobSystem = nullptr;
	// JPH::TempAllocatorImpl* PhysicsSystem::sTempAllocator = nullptr;

	void JoltTrace(const char* inFMT, ...) {
		va_list args;
		va_start(args, inFMT);

		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, args);
		va_end(args);

		LX_CORE_TRACE("[Physics] {}", buffer);
	}

	bool JoltAssertFailed(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine) {
		LX_CORE_ERROR("[Physics]:\nExpression: {}\nMessage: {}\nFile: {}:{}", inExpression, (inMessage ? inMessage : "None"), inFile, inLine);
		return true;
	}

	// set the funcs
	void PhysicsSystem::Initialize() {
		// Set the default logging stuff for JPH
		// JPH::Trace = JoltTrace;
		// JPH::AssertFailed = JoltAssertFailed;

		// Create the job system (multithreaded)
		// JPH::uint numThreads = std::thread::hardware_concurrency() - 1;
		// static constexpr JPH::uint maxJobs = 2048;
		// static constexpr JPH::uint maxBarriers = 16;
		// sJobSystem = new JPH::JobSystemThreadPool(maxJobs, maxBarriers, numThreads);


		// Set temp allocator (10mb)
		// sTempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
	}


	void PhysicsSystem::Shutdown() {
		//delete sJobSystem;
		//sJobSystem = nullptr;
		//
		//delete sTempAllocator;
		//sTempAllocator = nullptr;

		//delete JPH::Factory::sInstance;
		//JPH::Factory::sInstance = nullptr;
	}

	std::unique_ptr<PhysicsWorld> PhysicsSystem::CreateWorld(const PhysicsWorldDesc& physicsWorldDesc) {
		std::unique_ptr<PhysicsWorld> world = std::make_unique<PhysicsWorld>();
		world->settings = physicsWorldDesc;
		return world;
	}
	void PhysicsSystem::DeleteWorld(PhysicsWorld* world) {
		// Uninitialize the world
	}
}