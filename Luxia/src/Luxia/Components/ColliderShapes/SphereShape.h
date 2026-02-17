#pragma once

#include "ColliderShape.h"
#include "Luxia/Physics/PhysicsWorld.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"


namespace Luxia::Collider {
	struct LUXIA_API SphereShape : public ColliderShape {
	public:
		virtual JPH::ShapeRefC InitCollider(glm::vec3& scale) override {
			this->scale = scale;
			return new JPH::SphereShape(scale.x);
		}

		virtual void OnInspectorDraw(glm::vec3& scale, glm::vec3& offset) override {
			ImGui::Text("Sphere Collider");
			float radius = scale.x;
			if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.0001f)) {
				scale.x = radius;
				scale.y = radius;
				scale.z = radius;
			}
			if (ImGui::DragFloat3("Offset", &offset.x, 0.1f, 0.0f)) {
			
			}
		}
	};
}