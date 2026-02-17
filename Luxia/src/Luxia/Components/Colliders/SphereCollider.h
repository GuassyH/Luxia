#pragma once

#include "Collider.h"
#include "Luxia/Physics/PhysicsWorld.h"
#include "Jolt/Physics/Collision/Shape/SphereShape.h"


namespace Luxia::Components {
	struct LUXIA_API SphereCollider : public Collider {
	public:
		float radius = 0.5f;
		JPH::ShapeRefC sphereShape = new JPH::SphereShape(radius);
		glm::vec3 offset = glm::vec3(0.0f);

		virtual JPH::ShapeRefC InitCollider() override {
			return sphereShape;
		}

		virtual void OnInspectorDraw() override {

			if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.0001f)) {
				sphereShape.GetPtr()->Release();
				sphereShape = new JPH::SphereShape(radius);
			}
			ImGui::DragFloat3("Offset", &offset.x, 0.1f, 0.0f);
		}
	};
}