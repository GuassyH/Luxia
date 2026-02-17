#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Physics/PhysicsWorld.h"
#include "glm/glm.hpp"
 
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

namespace Luxia::Collider {

	class LUXIA_API ColliderShape {
	public:
		glm::vec3 scale;

		virtual ~ColliderShape() = default;
		virtual JPH::ShapeRefC InitCollider(glm::vec3& scale, glm::vec3& offset) = 0;
		virtual void OnInspectorDraw(glm::vec3& scale, glm::vec3& offset) = 0;
	};

}