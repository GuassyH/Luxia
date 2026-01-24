#pragma once

#include "Component.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace Luxia::Components {
	struct LUXIA_API RigidBody : public Component {
	public:
		JPH::Body* body;

		JPH::Vec3 halfExtent = JPH::Vec3(0.5f, 0.5f, 0.5f);
		JPH::ShapeRefC boxShape = new JPH::BoxShape(halfExtent);

		JPH::EMotionType motionType = JPH::EMotionType::Dynamic;

		void CalculatePosition(glm::vec3 new_world_pos) {
			transform->local_position += new_world_pos - transform->world_position;
		}

		void CalculateRotation(glm::quat new_world_rotation) {
			// do NOTHING right now
			transform->local_euler_angles += glm::degrees(glm::eulerAngles(new_world_rotation)) - transform->world_euler_angles;
		}

		virtual void OnInspectorDraw() override {
			const char* items[] = { "Static", "Kinematic", "Dynamic" };
			int current_item = static_cast<int>(motionType);
			if (ImGui::Combo("Motion Type", &current_item, items, IM_ARRAYSIZE(items))) {
				motionType = static_cast<JPH::EMotionType>(current_item);
			}
		}
	};
}