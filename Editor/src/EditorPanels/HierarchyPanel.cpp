#include "HierarchyPanel.h"
#include "EditorLayer.h"

namespace Editor::Panels {
	void HierarchyPanel::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Hierarchy Panel: Init");
	}

	std::vector<Luxia::GUID> entitiesToDelete;
	void HierarchyPanel::DrawEntitySelectable(Luxia::Entity& entity, Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
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

		// DRAGGING AND DROPPING
		if (editorLayer->isOneSelected && editorLayer->selected_assets.contains(entity.guid)) {
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload("ENTITY_ITEM", &entity.guid, sizeof(uint64_t));
				ImGui::Text(entity.name.c_str());
				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const auto payload = ImGui::AcceptDragDropPayload("ENTITY_ITEM")) {
				LX_CORE_ASSERT(payload->DataSize == sizeof(uint64_t));
				Luxia::GUID payloadGUID = *static_cast<uint64_t*>(payload->Data);

				if (scene->runtime_entities.contains(payloadGUID) && payloadGUID != entity.guid) {
					auto& payloadEnt = scene->runtime_entities.find(payloadGUID)->second;
					payloadEnt.transform->SetParent(entity.transform);
				}
				else {
					LX_ERROR("Hierarchy Panel: Scene does not contain guid - {}", (uint64_t)payloadGUID);
				}
			}
		
			ImGui::EndDragDropTarget();
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

	static Luxia::Entity& CreateHierarchyEntity(std::string name, Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
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

	void HierarchyPanel::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Hierarchy Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Hierarchy Panel";
		TabPopupContextMenu();

		if (!scene) { ImGui::End(); return; }

		static bool openRenamePopup = false;

		for (auto& [guid, entity] : scene->runtime_entities) {
			if (entity.transform) {
				if (!entity.transform->HasParent()) {
					DrawEntitySelectable(entity, editorLayer, scene);
				}
			}
		}


		if (ImGui::GetContentRegionAvail().y > 0.0f) {
			ImGui::InvisibleButton("HierarchyEmptySpace", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
			if (ImGui::IsWindowHovered()) {
				if (ImGui::IsItemHovered()) {
					if (ImGui::GetIO().MouseClicked[0] || ImGui::GetIO().MouseClicked[1]) {
						editorLayer->ClearSelected();
					}
				}
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ITEM")) {
					LX_CORE_ASSERT(payload->DataSize == sizeof(uint64_t));
					Luxia::GUID payloadGUID = *static_cast<const uint64_t*>(payload->Data);

					if (scene->runtime_entities.contains(payloadGUID)) {
						auto& ent = scene->runtime_entities.at(payloadGUID);

						// DROP ON EMPTY SPACE → UNPARENT
						ent.transform->SetParent(nullptr);
					}
				}
				ImGui::EndDragDropTarget();
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
					auto& new_entity = CreateHierarchyEntity("Directional Light", editorLayer, scene);
					auto& light = new_entity.transform->AddComponent<Luxia::Components::Light>();
					light.lightType = Luxia::LightType::Directional;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Spot Light")) {
					auto& new_entity = CreateHierarchyEntity("Spot Light", editorLayer, scene);
					auto& light = new_entity.transform->AddComponent<Luxia::Components::Light>();
					light.lightType = Luxia::LightType::Spot;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Point Light")) {
					auto& new_entity = CreateHierarchyEntity("Point Light", editorLayer, scene);
					auto& light = new_entity.transform->AddComponent<Luxia::Components::Light>();
					light.lightType = Luxia::LightType::Point;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Area Light")) {
					auto& new_entity = CreateHierarchyEntity("Area Light", editorLayer, scene);
					auto& light = new_entity.transform->AddComponent<Luxia::Components::Light>();
					light.lightType = Luxia::LightType::Area;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndMenu();
			}
				
			if (ImGui::BeginMenu("3D Objects")) {
				if (ImGui::MenuItem("Cube")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Cube", editorLayer, scene);
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					mr.mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(Luxia::ResourceManager::DefaultCube->guid);
					mr.material = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(Luxia::ResourceManager::DefaultLitMaterial->guid);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sphere")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Sphere", editorLayer, scene);
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					mr.mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(Luxia::ResourceManager::DefaultSphere->guid);
					mr.material = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(Luxia::ResourceManager::DefaultLitMaterial->guid);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Plane")) {
					// Does Nothing
					auto& new_entity = CreateHierarchyEntity("Plane", editorLayer, scene);
					auto& mr = new_entity.transform->AddComponent<Luxia::Components::MeshRenderer>();
					mr.mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(Luxia::ResourceManager::DefaultPlane->guid);
					mr.material = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(Luxia::ResourceManager::DefaultLitMaterial->guid);
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
	void HierarchyPanel::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		entitiesToDelete.clear();
	}

	void HierarchyPanel::OnEvent(Luxia::Event& e) {
		// Luxia::EventDispatcher dispatcher(e);
	}
}