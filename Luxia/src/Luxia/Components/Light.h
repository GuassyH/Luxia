#pragma once

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API Light : public Component {
	public:
		glm::vec4 color = glm::vec4(1.0f);

		virtual void OnInspectorDraw() override {
			ImGui::ColorPicker4("Color", &color.r);
		}
	};
}