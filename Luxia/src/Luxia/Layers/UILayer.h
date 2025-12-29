#pragma once

#include "Layer.h"
#include "Luxia/Core/Core.h"

namespace Luxia::Layers {
	class LUXIA_API UILayer : public Luxia::Layer
	{
	public:
		UILayer() {	name = "UILayer";	}
		~UILayer() = default;

		int flags =
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus;

		glm::vec2 size;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;
	};
}

