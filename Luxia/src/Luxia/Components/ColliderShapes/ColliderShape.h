#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>

namespace Luxia::Collider {

	class ColliderShape {
	public:
		glm::vec3 scale;

		virtual ~ColliderShape() = default;
		virtual JPH::ShapeRefC InitCollider(glm::vec3& scale) = 0;
		virtual void OnInspectorDraw(glm::vec3& scale, glm::vec3& offset) = 0;
	};

}