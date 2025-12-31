#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/GUID.h"

#include <iostream>
#include <entt/entt.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Luxia::Components {
	struct Transform;

	struct LUXIA_API Component {
		// data
		Transform* transform;
		Luxia::GUID ent_guid = Luxia::GUID(0);

		virtual void OnInspectorDraw() {}
	};
}