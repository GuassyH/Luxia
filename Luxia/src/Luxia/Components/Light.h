#pragma once

#include "Component.h"

namespace Luxia {
	enum LightType {
		Directional,
		Spot,
		Point,
		Area
	};
}

namespace Luxia::Components {
	struct LUXIA_API Light : public Component {
	public:
		glm::vec4 color = glm::vec4(1.0f);
		Luxia::LightType lightType = LightType::Spot;
		float intensity = 1.0f;

		virtual void OnInspectorDraw() override {
			const char* items[] = { "Directional", "Spot", "Point", "Area" };
			int current_item = static_cast<int>(lightType);
			if (ImGui::Combo("Light Type", &current_item, items, IM_ARRAYSIZE(items))) {
				lightType = static_cast<Luxia::LightType>(current_item);
			}

			ImGui::ColorEdit4("Color", &color.r);
		}
	};
}