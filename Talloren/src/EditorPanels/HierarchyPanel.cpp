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


		ImGui::SetCursorPosX(15 * level);
		ImGui::Separator();
		
		m_level++;
		for (auto child : entity.transform->children) {
			if (!child) continue;
			auto it = scene->runtime_entities.find(child->ent_guid);
			if (it != scene->runtime_entities.end()) {
				DrawEntitySelectable(it->second, editorLayer, scene, m_level);
			}
		}

	}

	static Luxia::Entity& CreateHierarchyEntity(std::string name, Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		auto& new_ntt = scene->CreateEntity(name);
		if (editorLayer->is_entity_selected) { // Set parent if there is a currently selected
			auto& parent_ent = scene->runtime_entities.find(editorLayer->selected_entity)->second;
			new_ntt.transform->SetParent(parent_ent.transform);
		}

		editorLayer->selected_entity = new_ntt.guid;
		editorLayer->is_entity_selected = true;
		return new_ntt;
	}

	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		if (ImGui::IsWindowHovered()) {
			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1) || Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_2)) {
				if (!ImGui::IsAnyItemHovered()) {
					editorLayer->is_entity_selected = false;
				}
			}
		}

		for (auto& [guid, entity] : scene->runtime_entities) {
			if (entity.transform) {
				if (!entity.transform->HasParent()) {
					DrawEntitySelectable(entity, editorLayer, scene, 0);
				}
			}
		}

		if (ImGui::BeginPopupContextWindow("Create Object", ImGuiPopupFlags_MouseButtonRight)) {
			// On Creation you should get the option to name the entity
			if (ImGui::MenuItem("Delete", (const char*)0, (bool*)0, editorLayer->is_entity_selected)) {
				entitiesToDelete.push_back(editorLayer->selected_entity);
				editorLayer->is_entity_selected = false;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Rename", (const char*)0, (bool*)0, editorLayer->is_entity_selected)) {
				// entity.name = thing;
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Create Empty")) {
				auto& new_entity = CreateHierarchyEntity("Entity", editorLayer, scene);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::BeginMenu("Lights")) {
				if (ImGui::MenuItem("Directional Light")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Directional Light", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Spot Light")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Spot Light", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Point Light")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Point Light", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Area Light")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Area Light", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("3D Objects")) {
				if (ImGui::MenuItem("Cube")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Cube", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sphere")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Sphere", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Quad")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Quad", editorLayer, scene);
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Camera")) {
				auto& new_entity = CreateHierarchyEntity("Camera", editorLayer, scene);
				new_entity.transform->AddComponent<Luxia::Components::Camera>(1920, 1080);
				ImGui::CloseCurrentPopup();
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