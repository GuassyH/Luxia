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
		const char* name = "unnamed_component";
		// data
		Transform* transform;
		Luxia::GUID ent_guid;

		virtual void OnInspectorDraw() {}
	};
}