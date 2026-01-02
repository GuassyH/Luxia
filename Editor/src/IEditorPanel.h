#pragma once

#include "Luxia.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


namespace Editor::Layers {
	class EditorLayer;
};
namespace Editor {
	class IEditorPanel {
	public:
		virtual ~IEditorPanel() = default;

		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) = 0;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) = 0;

		bool opened = true;
		std::string& GetName() { return name; }

		virtual void OnEvent(Luxia::Event& e) = 0;

		void TabPopupContextMenu() {
					
			if (ImGui::BeginPopupContextItem((std::string("TabContext_") + name).c_str())) {
				if (ImGui::MenuItem("Close"))
					opened = false;
				ImGui::EndPopup();
			}
		}

	protected:
		std::string name = "Editor Panel";
	};
};
