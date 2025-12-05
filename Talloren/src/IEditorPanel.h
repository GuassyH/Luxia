#pragma once

#include "Luxia.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


namespace Talloren::Layers {
	class EditorLayer;
};
namespace Talloren {
	class IEditorPanel {
	public:
		virtual ~IEditorPanel() = default;

		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) = 0;
		virtual void Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) = 0;

		bool opened = true;

		virtual void OnEvent(Luxia::Event& e) = 0;
	};
};
