#pragma once

#include "Luxia/Components/Component.h"

#include "ColliderShapes/ColliderShape.h"
#include "ColliderShapes/BoxShape.h"
#include "ColliderShapes/SphereShape.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace Luxia::Collider {
	enum ColliderType {
		Box,
		Sphere
	};
};

namespace Luxia::Components {
	struct LUXIA_API Collider : public Component {
	public:

		JPH::ShapeRefC shape = nullptr;
		std::unique_ptr<Luxia::Collider::ColliderShape> colliderShape;
		Luxia::Collider::ColliderType colliderType = Luxia::Collider::ColliderType::Box;

		glm::vec3 offset = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(0.5f);

		Collider() = default;
		Collider(Luxia::Collider::ColliderType type) : colliderType(type) {
			InitCollider();
		}

		// Depending on collider type, set collider shape, and return its shape reference
		void InitCollider() {
			ReleaseCollider(); // Release existing collider if any

			// Create correct collider shape based on type
			switch (colliderType) {
				case Luxia::Collider::ColliderType::Box:
					colliderShape = std::make_unique<Luxia::Collider::BoxShape>();
					break;
				case Luxia::Collider::ColliderType::Sphere:
					colliderShape = std::make_unique<Luxia::Collider::SphereShape>();
					break;
				default:
					break;
			}

			// Return the shape reference from the collider shape
			if (colliderShape) {
				shape = colliderShape->InitCollider(scale);
			}
		}
		void ReleaseCollider() {
			if (shape) 
				shape.GetPtr()->Release();
			
			shape = nullptr;
			colliderShape.release();
			colliderShape.reset();
		}

		void OnInspectorDraw() override {
			if (colliderShape) {
				colliderShape->OnInspectorDraw(scale, offset);
			}
		}
	};
};