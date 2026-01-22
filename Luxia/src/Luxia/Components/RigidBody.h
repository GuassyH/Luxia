#pragma once

#include "Component.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace Luxia::Components {
	struct LUXIA_API RigidBody : public Component {
	public:
		JPH::BodyID bodyID;

		JPH::Vec3 halfExtent = JPH::Vec3(0.5f, 0.5f, 0.5f);
		JPH::ShapeRefC boxShape = new JPH::BoxShape(halfExtent);


		void CalculatePosition(glm::vec3 new_world_pos) {
			transform->local_position += new_world_pos - transform->world_position;
		}

		void CalculateRotation(glm::quat new_world_rotation) {
			// do NOTHING right now
		}

		virtual void OnInspectorDraw() override {

		}
	};
}