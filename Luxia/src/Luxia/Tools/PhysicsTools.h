#pragma once

// Luxia API
#include "Luxia/Core/Log.h"
#include "Luxia/Core/Core.h"
#include "Luxia/Tools/Constants.h"

#include "glm/glm.hpp"

#define JPH_OVERRIDE_NEW_DELETE
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/Body.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/PhysicsSystem.h"

namespace Luxia::Physics {

	static struct RayCastHit {
		float distance;
		bool hit = false;
		glm::vec3 position;
		glm::vec3 normal;
		JPH::BodyID bodyID;
	};

};