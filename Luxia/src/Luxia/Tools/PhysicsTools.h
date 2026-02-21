#pragma once

// Luxia API
#include "Luxia/Core/Log.h"
#include "Luxia/Core/Core.h"
#include "Luxia/Tools/Constants.h"

#include "glm/glm.hpp"

 
#include "Jolt/Jolt.h"
#include "Jolt/Physics/Body/BodyID.h"
#include "Jolt/Physics/Body/Body.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/PhysicsSystem.h"

namespace Luxia::Physics {

	static struct Ray {
		glm::vec3 origin;
		glm::vec3 direction;
		float length;
	};

	static struct RayCastHit {
		float distance = 0.0f;
		bool hit = false;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		Ray ray;
		JPH::BodyID bodyID = JPH::BodyID(0);
	};


	static bool ClosestPointOnLineToRay (
		const glm::vec3& linePoint,
		const glm::vec3& lineDir,   // normalize this
		const glm::vec3& rayOrigin,
		const glm::vec3& rayDir,    // normalize this
		glm::vec3& outClosestPoint
	) {
		glm::vec3 w0 = linePoint - rayOrigin;

		float a = glm::dot(lineDir, lineDir); // = 1
		float b = glm::dot(lineDir, rayDir);
		float c = glm::dot(rayDir, rayDir);   // = 1
		float d = glm::dot(lineDir, w0);
		float e = glm::dot(rayDir, w0);

		float denom = a * c - b * b;

		float t, s;

		// If not parallel
		if (fabs(denom) > 1e-6f) {
			t = (b * e - c * d) / denom;
			s = (a * e - b * d) / denom;
		}
		else {
			// Parallel: pick arbitrary closest
			t = -d / a;
			s = 0.0f;
		}

		// Enforce ray constraint
		if (s < 0.0f) {
			s = 0.0f;
			t = -d / a; // project ray origin onto line
		}

		outClosestPoint = linePoint + t * lineDir;
		return true;
	}
};