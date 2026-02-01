#pragma once

#include "Luxia.h"
#include "glm/glm.hpp"
#include "Luxia/Managers/ResourceManager.h"
#include "Luxia/Scene/Scene.h"
#include "Luxia/Managers/AssetManager.h"

namespace Editor::Scripts {

	class SceneShooterTest : public Luxia::Components::Component {
	public:
		float speed = 1.0f;

		void Shoot(Luxia::Scene& scene, Luxia::AssetManager* asset_manager) {
			if (!transform) { return; }

			// Create a new entity
			Luxia::Entity& bullet = scene.CreateEntity("Bullet");

			// Set its transform to be in front of the camera
			bullet.transform->local_position = transform->local_position + transform->forward;
			bullet.transform->local_rotation = transform->local_rotation;
			bullet.transform->local_scale = glm::vec3(0.2f);

			bullet.transform->UpdateMatrix();

			// Add a MeshRenderer component (assuming a sphere mesh and basic material exist)
			auto& meshRend = bullet.transform->AddComponent<Luxia::Components::MeshRenderer>();
			meshRend.mesh = asset_manager->GetAsset<Luxia::Mesh>(Luxia::ResourceManager::DefaultCube->guid);
			meshRend.material = asset_manager->GetAsset<Luxia::IMaterial>(Luxia::ResourceManager::DefaultLitMaterial->guid);

			// Add a RigidBody component
			auto& rb = bullet.transform->AddComponent<Luxia::Components::RigidBody>();
			rb.motionType = JPH::EMotionType::Dynamic;
			rb.motionQuality = JPH::EMotionQuality::LinearCast;
			rb.mass = 0.05f;
			rb.gravityScale = 0.0f; // No gravity for bullets

			// Add BoxCollider component
			auto& boxCollider = bullet.transform->AddComponent<Luxia::Components::BoxCollider>();
			boxCollider.halfExtent = Luxia::Physics::ToJolt(glm::vec3(0.05f));

			// Initialize the rigid body
			rb.InitBody(scene.physicsWorld->jphSystem.GetBodyInterface());
				
			LX_CORE_INFO("Shot: {}x {}y {}z", bullet.transform->local_position.x, bullet.transform->local_position.y, bullet.transform->local_position.z);

			// Set the bullet's velocity forward
			rb.body->SetLinearVelocity(Luxia::Physics::ToJolt(transform->forward * speed * 10.0f));
		}
	};

}