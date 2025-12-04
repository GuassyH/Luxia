#pragma once

#include "Luxia.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


namespace Talloren::Layers {
	class EditorLayer;
};
namespace Talloren::Editor {
	class IEditorPanel {
	public:
		virtual ~IEditorPanel() = default;

		virtual void Init() {}
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer) = 0;
		virtual void Unload() = 0;

		virtual void OnEvent(Luxia::Event& e) = 0;
	};
};
