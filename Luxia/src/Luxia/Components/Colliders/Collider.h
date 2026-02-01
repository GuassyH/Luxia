#pragma once

#include "Luxia/Components/Component.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

namespace Luxia::Components {
	struct LUXIA_API Collider : public Component {
	public:
		virtual JPH::ShapeRefC InitCollider() = 0;

		virtual void OnInspectorDraw() override = 0;
	};
}