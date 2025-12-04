#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Talloren::Editor::Panel {
	void HierarchyPanel::Init() {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}
	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		auto view = scene->GetEntitiesWith<Luxia::Components::Transform>();
		bool selected = false;
		for (auto entity : view) {

			std::ostringstream s; s << (int)entity;
			ImGui::Selectable(s.str().c_str(), &selected);
		}

		ImGui::End();
	}
	void HierarchyPanel::Unload() {

	}

	void HierarchyPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}