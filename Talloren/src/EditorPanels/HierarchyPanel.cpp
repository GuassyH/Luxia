#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Talloren::Editor::Panel {
	void HierarchyPanel::Init() {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}

	void HierarchyPanel::DrawEntitySelectable(Luxia::Components::Transform* entity, Talloren::Layers::EditorLayer* editorLayer, int level) {
		int m_level = level;
		std::ostringstream s; s <<  "> " << (int)entity->ent_id;

		ImGui::SetCursorPosX(15 * level);
		if (ImGui::Selectable(s.str().c_str(), editorLayer->selected_entity == entity->ent_id && editorLayer->is_entity_selected))
		{
			editorLayer->selected_entity = entity->ent_id;
			editorLayer->is_entity_selected = true;
		}

		m_level++;
		for (auto child : entity->children)
		{
			if (!child) continue;
			DrawEntitySelectable(child, editorLayer, m_level);
		}
	}


	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		auto view = scene->GetEntitiesWith<Luxia::Components::Transform>();
		for (auto entity : view) {
			auto t = scene->TryGetFromEntity<Luxia::Components::Transform>(entity);

			if (t) {
				if (!t->HasParent()) {
					DrawEntitySelectable(t, editorLayer, 0);
				}
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