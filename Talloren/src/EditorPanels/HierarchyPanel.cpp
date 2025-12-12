#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void HierarchyPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}

	void HierarchyPanel::DrawEntitySelectable(Luxia::Entity& entity, Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, int level) {

		int m_level = level;
		std::ostringstream s; s << "> " << entity.name << "##" << std::to_string(entity.guid);

		ImGui::SetCursorPosX(15 * level);
		if (ImGui::Selectable(s.str().c_str(), editorLayer->selected_entity == entity.guid && editorLayer->is_entity_selected))
		{
			editorLayer->selected_entity = entity.guid;
			editorLayer->is_entity_selected = true;
		}

		ImGui::SetCursorPosX(15 * level);
		ImGui::Separator();
		
		m_level++;
		for (auto child : entity.transform->children)
		{
			if (!child) continue;
			DrawEntitySelectable(scene->runtime_entities.find(child->ent_guid)->second, editorLayer, scene, m_level);
		}

	}


	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		for (auto& [guid, entity] : scene->runtime_entities) {
			if (entity.transform) {
				if (!entity.transform->HasParent()) {
					DrawEntitySelectable(entity, editorLayer, scene, 0);
				}
			}
		}

		if (ImGui::IsWindowHovered()) {
			if (!ImGui::IsItemHovered()) {
				if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_1)) {
					editorLayer->is_entity_selected = false;
				}
			}
		}

		// ImGui::Separator();
		ImGui::End();
	}
	void HierarchyPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void HierarchyPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}