#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void HierarchyPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}

	std::vector<Luxia::GUID> entitiesToDelete;
	void HierarchyPanel::DrawEntitySelectable(Luxia::Entity& entity, Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, int level) {

		int m_level = level;
		std::ostringstream enttext; enttext << "> " << entity.name << "##" << std::to_string(entity.guid);

		// CAN be collapsing header
		ImGui::SetCursorPosX(15 * level);
		ImGui::Selectable(enttext.str().c_str(), editorLayer->selected_entity == entity.guid && editorLayer->is_entity_selected);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			editorLayer->selected_entity = entity.guid;
			editorLayer->is_entity_selected = true;
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			editorLayer->selected_entity = entity.guid;
			editorLayer->is_entity_selected = true;
		}

		std::ostringstream changeent; changeent << "Change Object##" << (uint64_t)entity.guid;
		if (ImGui::BeginPopupContextItem(changeent.str().c_str())) {
			if (ImGui::MenuItem("Delete")) {
				entitiesToDelete.push_back(entity.guid);
				editorLayer->is_entity_selected = false;
				ImGui::EndPopup();
				return;
			} 
			if (ImGui::MenuItem("Rename")) {
				// entity.name = thing;
				ImGui::EndPopup();
				return;
			}

			ImGui::EndPopup();
		}


		ImGui::SetCursorPosX(15 * level);
		ImGui::Separator();
		
		m_level++;
		for (auto child : entity.transform->children) {
			if (!child) continue;
			DrawEntitySelectable(scene->runtime_entities.find(child->ent_guid)->second, editorLayer, scene, m_level);
		}

	}


	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		/*
		if (!ImGui::IsAnyItemHovered() && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup)){
			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
				editorLayer->is_entity_selected = false;
			}
		}
		*/

		for (auto& [guid, entity] : scene->runtime_entities) {
			if (entity.transform) {
				if (!entity.transform->HasParent()) {
					DrawEntitySelectable(entity, editorLayer, scene, 0);
				}
			}
		}

		if (ImGui::BeginPopupContextWindow("Create Object", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::BeginMenu("Create")) {
				if (ImGui::MenuItem("Empty")) {
					auto& new_ntt = scene->CreateEntity();
					editorLayer->selected_entity = new_ntt.guid;
					editorLayer->is_entity_selected = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		for (auto ent : entitiesToDelete) {
			scene->DeleteEntity(ent);
		}
		entitiesToDelete.clear();

		ImGui::End();
	}
	void HierarchyPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void HierarchyPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}