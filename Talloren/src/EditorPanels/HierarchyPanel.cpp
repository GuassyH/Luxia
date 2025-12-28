#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panels {
	void HierarchyPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}

	std::vector<Luxia::GUID> entitiesToDelete;
	void HierarchyPanel::DrawEntitySelectable(Luxia::Entity& entity, Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		std::ostringstream enttext; enttext << entity.name;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		if (editorLayer->selected_assets.contains(entity.guid)) flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(std::to_string((uint64_t)entity.guid).c_str());
		bool open = ImGui::TreeNodeEx(enttext.str().c_str(), flags);
		ImGui::PopID();

		bool is_selected = editorLayer->selected_assets.contains(entity.guid);
		if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) && !ImGui::IsItemToggledOpen()) {
			if (ImGui::GetIO().KeyCtrl) {
				if (is_selected)
					editorLayer->EraseSelected(entity.guid);
				else
					editorLayer->InsertSelected(entity.guid);
			}
			else {
				editorLayer->ClearSelected();
				editorLayer->InsertSelected(entity.guid);
			}
		}

		ImGui::Separator();

		if (open) {
			for (auto child : entity.transform->children) {
				if (!child) continue;
				auto it = scene->runtime_entities.find(child->ent_guid);
				if (it != scene->runtime_entities.end()) {
					DrawEntitySelectable(it->second, editorLayer, scene);
				}
			}
			ImGui::TreePop();
		}
	}

	static Luxia::Entity& CreateHierarchyEntity(std::string name, Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		auto& new_ntt = scene->CreateEntity(name);
		if (editorLayer->isOneSelected) { // Set parent if there is a currently selected
			Luxia::GUID e_guid = *editorLayer->selected_assets.begin();

			if (scene->runtime_entities.contains(e_guid)) {
				auto& parent_ent = scene->runtime_entities.find(e_guid)->second;
				new_ntt.transform->SetParent(parent_ent.transform);
			}
		}

		editorLayer->ClearSelected();
		editorLayer->InsertSelected(new_ntt.guid);
		return new_ntt;
	}

	void HierarchyPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel");

		if (!scene) { ImGui::End(); return; }

		static bool openRenamePopup = false;

		if (ImGui::IsWindowHovered()) {
			if (ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1]) {
				if (!ImGui::IsAnyItemHovered()) {
					editorLayer->ClearSelected();
				}
			}
		}

		for (auto& [guid, entity] : scene->runtime_entities) {
			if (entity.transform) {
				if (!entity.transform->HasParent()) {
					DrawEntitySelectable(entity, editorLayer, scene);
				}
			}
		}

		if (ImGui::BeginPopupContextWindow("Create Object", ImGuiPopupFlags_MouseButtonRight)) {
			// On Creation you should get the option to name the entity
			if (ImGui::MenuItem("Delete", nullptr, nullptr, editorLayer->isOneSelected || editorLayer->areMultipleSelected)) {
				for (auto& guid : editorLayer->selected_assets) {
					entitiesToDelete.push_back(guid);
					ImGui::CloseCurrentPopup();
				}
			}
			if (ImGui::MenuItem("Rename", nullptr, nullptr, editorLayer->isOneSelected || editorLayer->areMultipleSelected)) {
				openRenamePopup = true;
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
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					// mr.material = Luxia::Resources::DefaultMaterial();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sphere")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Sphere", editorLayer, scene);
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					// mr.material = Luxia::Resources::DefaultMaterial();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Quad")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Quad", editorLayer, scene);
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					// mr.material = Luxia::Resources::DefaultMaterial();
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

		// Rename Popup
		if (openRenamePopup) {
			ImGui::OpenPopup("Rename Entity Popup");
			openRenamePopup = false;
		}

		static char nameBuffer[256];
		static bool init = false;

		if (ImGui::BeginPopupModal("Rename Entity Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			auto guid = *editorLayer->selected_assets.begin();

			if (!init) {
				auto& ent = scene->runtime_entities.at(guid);
				strcpy_s(nameBuffer, ent.name.c_str());
				init = true;
			}

			ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer));

			if (ImGui::Button("Rename")) {
				auto& ent = scene->runtime_entities.at(guid);
				ent.name = nameBuffer;
				init = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")) {
				init = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}


		// Delete Entities
		for (auto& ent : entitiesToDelete) {
			scene->DeleteEntity(ent);
		}
		entitiesToDelete.clear();

		ImGui::End();
	}
	void HierarchyPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		entitiesToDelete.clear();

	}

	void HierarchyPanel::OnEvent(Luxia::Event& e) {
		// Luxia::EventDispatcher dispatcher(e);
	}
}