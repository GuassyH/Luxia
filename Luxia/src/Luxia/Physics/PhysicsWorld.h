#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "glm/glm.hpp"
#include "PhysicsSystem.h"

#define JPH_OVERRIDE_NEW_DELETE
#include "Jolt/Jolt.h"

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/PhysicsSystem.h>

// These were taken from JoltPhysics HelloWorld to learn about Jolt
namespace Luxia::Physics {
	/// Layers that objects can be in, defines what can collide with what.
	namespace Layers {
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;

	};

	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
		{
			switch (inObject1)
			{
			case Layers::NON_MOVING:
				return inObject2 == Layers::MOVING; // Non moving only collides with moving
			case Layers::MOVING:
				return true; // Moving collides with everything
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	/// Each Broadphase layer results in a bounding volume tree. At least one layer for moving and non
	namespace BroadPhaseLayers {
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr JPH::uint NUM_LAYERS(2);
	}


	// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			// Create a mapping table from object to broad phase layer
			mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
			mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
		}

		virtual JPH::uint GetNumBroadPhaseLayers() const override
		{
			return BroadPhaseLayers::NUM_LAYERS;
		}

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
			return mObjectToBroadPhase[inLayer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
		{
			switch ((JPH::BroadPhaseLayer::Type)inLayer)
			{
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
			case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
			default:													JPH_ASSERT(false); return "INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
			case Layers::NON_MOVING:
				return inLayer2 == BroadPhaseLayers::MOVING;
			case Layers::MOVING:
				return true;
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	// An example contact listener
	class MyContactListener : public JPH::ContactListener
	{
	public:
		// See: ContactListener
		virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
		{
			LX_CORE_INFO("[Physics]: Contact validate callback");

			// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
			return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
		}

		virtual void			OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
		{
			LX_CORE_INFO("[Physics]: A contact was added");
		}

		virtual void			OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
		{
			LX_CORE_INFO("[Physics]: A contact was persisted");
		}

		virtual void			OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
		{
			LX_CORE_INFO("[Physics]: A contact was removed");
		}
	};

	// An example activation listener
	class MyBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void		OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
		{
			LX_CORE_INFO("[Physics]: A body got activated");
		}

		virtual void		OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override
		{
			LX_CORE_INFO("[Physics]: A body went to sleep");
		}
	};
}


namespace Luxia::Physics {

	inline glm::vec3 ToGLM(const JPH::Vec3& v) {
		return glm::vec3(v.GetX(), v.GetY(), v.GetZ());
	}

	inline JPH::Vec3 ToJolt(const glm::vec3& v) {
		return JPH::Vec3(v.x, v.y, v.z);
	}

	inline glm::quat ToGLM(const JPH::Quat& q) {
		// JPH::Quat stores (w, x, y, z)
		return glm::quat(q.GetW(), q.GetX(), q.GetY(), q.GetZ());
	}

	inline JPH::Quat ToJolt(const glm::quat& q) {
		// glm::quat stores (w, x, y, z)
		return JPH::Quat(q.w, q.x, q.y, q.z);
	}

	struct PhysicsWorldDesc {
		glm::vec3 gravity = glm::vec3(0.0f, -9.82f, 0.0f);

		const unsigned int cMaxBodies = 1024;
		const unsigned int cNumBodyMutexes = 0;
		const unsigned int cMaxBodyPairs = 1024;
		const unsigned int cMaxContactConstraints = 1024;
	};


	class PhysicsWorld {
	public:
		PhysicsWorld() = default;
		~PhysicsWorld() = default;

		JPH::PhysicsSystem jphSystem;
		BPLayerInterfaceImpl broadPhaseLayerInterface;
		ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
		ObjectLayerPairFilterImpl objectLayerPairFilter;
		MyBodyActivationListener bodyActivationListener;
		MyContactListener contactListener;

		void Initialize(const PhysicsWorldDesc& PWD) {

			jphSystem.Init(PWD.cMaxBodies, PWD.cNumBodyMutexes, PWD.cMaxBodyPairs, PWD.cMaxContactConstraints, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
			jphSystem.SetBodyActivationListener(&bodyActivationListener);
			jphSystem.SetContactListener(&contactListener);
			jphSystem.SetGravity(JPH::Vec3(PWD.gravity.x, PWD.gravity.y, PWD.gravity.z));

		}

		void step(float dt) {
			int step = 1; // Should be 1 step per 1/60th seconds according to JPH
			jphSystem.Update(dt, step, Luxia::Physics::PhysicsSystem::sTempAllocator, Luxia::Physics::PhysicsSystem::sJobSystem);
		}

		PhysicsWorldDesc settings;

	};

}