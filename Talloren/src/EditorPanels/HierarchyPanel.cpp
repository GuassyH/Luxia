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
		for (auto entity : view) {
			auto t = scene->TryGetFromEntity<Luxia::Components::Transform>(entity);

			if (t) {
				std::ostringstream s; s << (int)entity;
				if(ImGui::Selectable(s.str().c_str(), editorLayer->selected_entity == entity && editorLayer->is_entity_selected))
				{
					editorLayer->selected_entity = entity;
					editorLayer->is_entity_selected = true;
				}
				// Go through each child and draw. 
				// Root node should ALWAYS draw first and THEN the child node
				// Recursion here maybe? Like with the matrix updates?
			}
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