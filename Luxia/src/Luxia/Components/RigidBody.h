#pragma once

#include "Component.h"
#include "Colliders/BoxCollider.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>


namespace Luxia::Components {
	struct LUXIA_API RigidBody : public Component {
	public:
		JPH::Body* body;

		float gravityScale = 1.0f;
		float mass = 1.0f;
		float angularDamping = 0.05f;
		float linearDamping = 0.05f;

		JPH::EMotionType motionType = JPH::EMotionType::Dynamic;
		JPH::EMotionQuality motionQuality = JPH::EMotionQuality::Discrete;

		void InitBody(JPH::BodyInterface& body_interface) {
			JPH::BodyCreationSettings settings;
			
			JPH::MassProperties mass_props;
			mass_props.mMass = mass;
			mass_props.mInertia = JPH::Mat44::sScale(JPH::Vec3(1.0f, 1.0f, 1.0f)); // Simple inertia, can be improved

			// Check for collider
			if(transform->HasComponent<Components::BoxCollider>()) {
				auto& bcollider = transform->GetComponent<Components::BoxCollider>();
				JPH::ShapeRefC shape = bcollider.InitCollider();
				settings.SetShape(shape);
				settings.mObjectLayer = Physics::Layers::MOVING;
			}
			else {
				settings.SetShape(new JPH::BoxShape(JPH::Vec3(0.5f, 0.5f, 0.5f)));
				settings.mObjectLayer = Physics::Layers::NON_COLLIDING;
			}

			// Set body settings
			settings.mMotionType = motionType;
			settings.mPosition = Physics::ToJolt(transform->world_position);
			settings.mRotation = Physics::ToJolt(transform->world_rotation);
			settings.mMotionQuality = motionQuality;
			settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
			settings.mMassPropertiesOverride = mass_props;
			settings.mAngularDamping = angularDamping;
			settings.mLinearDamping = linearDamping;
			settings.mGravityFactor = gravityScale;
			settings.mFriction = 0.5f;
			settings.mRestitution = 0.1f;

			body = body_interface.CreateBody(settings);

			body_interface.AddBody(body->GetID(), JPH::EActivation::Activate);
		}

		void CalculatePosition(glm::vec3 new_world_pos) {
			transform->local_position += new_world_pos - transform->world_position;
		}

		void CalculateRotation(glm::quat new_world_rotation) {
			glm::quat delta = glm::normalize(new_world_rotation) * glm::inverse(transform->world_rotation);
			transform->local_rotation = glm::normalize(delta * transform->local_rotation);
		}

		virtual void OnInspectorDraw() override {
			const char* mt_items[] = { "Static", "Kinematic", "Dynamic" };
			int mt_current_item = static_cast<int>(motionType);
			if (ImGui::Combo("Motion Type", &mt_current_item, mt_items, IM_ARRAYSIZE(mt_items))) {
				motionType = static_cast<JPH::EMotionType>(mt_current_item);
			}

			const char* mq_items[] = { "Discrete", "Linear Cast" };
			int mq_current_item = static_cast<int>(motionQuality);
			if (ImGui::Combo("Motion Quality", &mq_current_item, mq_items, IM_ARRAYSIZE(mq_items))) {
				motionQuality = static_cast<JPH::EMotionQuality>(mq_current_item);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			
			ImGui::DragFloat("Mass", &mass, 0.1f, 0.1f);
			ImGui::DragFloat("Angular Damping", &angularDamping, 0.01f, 0.0f);
			ImGui::DragFloat("Linear Damping", &linearDamping, 0.01f, 0.0f);
			ImGui::DragFloat("Gravity Scale", &gravityScale, 0.1f);
			
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (body) {
				ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", body->GetLinearVelocity().GetX(), body->GetLinearVelocity().GetY(), body->GetLinearVelocity().GetZ());
				ImGui::Text("Angular Velocity: (%.2f, %.2f, %.2f)", body->GetAngularVelocity().GetX(), body->GetAngularVelocity().GetY(), body->GetAngularVelocity().GetZ());
			}
			else {
				ImGui::Text("Velocity: (0.0f, 0.0f, 0.0f)");
				ImGui::Text("Angular Velocity: (0.0f, 0.0f, 0.0f)");
			}
		}
	};
}