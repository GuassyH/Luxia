#pragma once

#include "ColliderShape.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"
#include "Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h"

namespace Luxia::Collider {
	struct LUXIA_API BoxShape : public ColliderShape {
	public:
		virtual JPH::ShapeRefC InitCollider(glm::vec3& scale, glm::vec3& offset) override {
			this->scale = scale;
			return new JPH::RotatedTranslatedShape(Physics::ToJolt(offset), JPH::Quat::sIdentity(), new JPH::BoxShape(Luxia::Physics::ToJolt(scale)));
		}

		virtual void OnInspectorDraw(glm::vec3& scale, glm::vec3& offset) override {
			ImGui::Text("Box Collider");
			if (ImGui::DragFloat3("Half Extent", &scale.x, 0.1f, 0.0001f)) {

			}
			if (ImGui::DragFloat3("Offset", &offset.x, 0.1f, 0.0f)) {

			}
		}
	};
}