#pragma once

#include "Collider.h"
#include "Luxia/Physics/PhysicsWorld.h"


namespace Luxia::Components {
	struct LUXIA_API BoxCollider : public Collider {
	public:
		JPH::Vec3 halfExtent = JPH::Vec3(0.5f, 0.5f, 0.5f);
		JPH::ShapeRefC boxShape = new JPH::BoxShape(halfExtent);
		glm::vec3 offset = glm::vec3(0.0f);
		
		virtual JPH::ShapeRefC InitCollider() override {
			return boxShape;
		}

		virtual void OnInspectorDraw() override {
			glm::vec3 ghalfExtent = Luxia::Physics::ToGLM(this->halfExtent);
			if (ImGui::DragFloat3("Half Extent", &ghalfExtent.x, 0.1f, 0.0f)) {
				this->halfExtent = Luxia::Physics::ToJolt(ghalfExtent);
				boxShape = new JPH::BoxShape(halfExtent);
			}
			ImGui::DragFloat3("Offset", &offset.x, 0.1f, 0.0f);
		}
	};
}