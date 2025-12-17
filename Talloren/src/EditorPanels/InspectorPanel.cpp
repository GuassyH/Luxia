#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}

	char namebuff[255];
	char meshbuff[255];
	char matbuff[255];
	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel");
		if (!editorLayer->is_entity_selected) { ImGui::End(); return; }
		if (!scene->runtime_entities.contains(editorLayer->selected_entity)) { ImGui::End(); return; }

		Luxia::Entity& ent = scene->runtime_entities.find(editorLayer->selected_entity)->second;


		// Name
		std::ostringstream entname; entname << ent.name;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(entname.str().c_str()).x * 0.5f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
		ImGui::SetNextItemWidth(180);
		if (ImGui::InputTextWithHint("##NameInput", entname.str().c_str(), namebuff, sizeof(namebuff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
			ent.name = namebuff;
			memset(namebuff, 0, sizeof(namebuff));
		}
		ImGui::SameLine();
		if (ImGui::Button("U")) {	// You can now delete entities but adding new ones after deleting doesnt work
			scene->DeleteEntity(ent.guid);
			editorLayer->is_entity_selected = false;
			ImGui::End();
			return;
		}

		// GUID
		std::ostringstream info; info << "Entity ID: " << (uint64_t)editorLayer->selected_entity;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.0f) - (ImGui::CalcTextSize(info.str().c_str()).x / 2.0f));
		ImGui::Text(info.str().c_str());


		// Draw transform component first
		if (ent.transform) {
			if (ImGui::CollapsingHeader(ent.transform->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				ent.transform->OnInspectorDraw();
			}
		}
		// Draw Other Components
		auto cam = ent.transform->TryGetComponent<Luxia::Components::Camera>();
		if (cam) {
			if (ImGui::CollapsingHeader(cam->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				cam->OnInspectorDraw();
			}
		}

		// Needs to be here to use asset_manager
		auto meshrend = ent.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
		if (meshrend) {
			if (ImGui::CollapsingHeader(meshrend->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				meshrend->OnInspectorDraw();

				// This needs to be here due to asset manager usage 

				int flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll |
					ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal;


				// MESH
				std::ostringstream mesht; mesht << "Mesh: ";
				if (!meshrend->mesh)
					mesht << "nullptr";
				else {
					mesht << meshrend->mesh->name;
				}

				ImGui::Text(mesht.str().c_str());

				std::string meshHint = meshrend->mesh ? std::to_string(meshrend->mesh->guid) : "0";
				ImGui::SameLine();
				if (ImGui::InputTextWithHint("##MeshInput", meshHint.c_str(), meshbuff, sizeof(meshbuff), flags)) {
					// meshguid = (uint64_t)meshbuff;
					Luxia::GUID meshguid(std::strtoull(meshbuff, nullptr, 10));

					if (editorLayer->GetAssetManager()->HasAsset<Luxia::Mesh>(meshguid))
						meshrend->mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(meshguid);
					else 
						LX_ERROR("Tried to assign asset that is not mesh or has invalid guid: {}", (uint64_t)meshguid);

					memset(meshbuff, 0, sizeof(meshbuff));
				}


				// MATERIAL
				std::ostringstream matt; matt << "Material: ";
				if (!meshrend->material)
					matt << "nullptr";
				else {
					matt << meshrend->material->name;
				}

				ImGui::Text(matt.str().c_str());


				std::string matHint = meshrend->material ? std::to_string(meshrend->material->guid) : "0";
				ImGui::SameLine();
				if (ImGui::InputTextWithHint("##MaterialInput", matHint.c_str(), matbuff, sizeof(matbuff), flags)) {
					// matguid = (uint64_t)matbuff;
					memset(matbuff, 0, sizeof(matbuff));
				}
			}
		}


		// TEMP ADD COMPONENT
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2.0f) - 50);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 50);
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("Add Component");
		}

		// should be menu
		float x = ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 3.0f);
		float y = ImGui::GetWindowPos().y + 1.5*(ImGui::GetWindowSize().y / 2.0f);
		ImGui::SetNextWindowPos(ImVec2(x, y));
		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImVec2 button_size = ImVec2(150, 20);

			// Temporarily do this manually, should go through all components automatically
			if (!ent.transform->HasComponent<Luxia::Components::Camera>()) {
				if (ImGui::Button("Camera Component", button_size)) {
					ent.transform->AddComponent<Luxia::Components::Camera>(1920, 1080);
					ImGui::CloseCurrentPopup();
				}
			}

			// Temporarily do this manually, should go through all components automatically
			if (!ent.transform->HasComponent<Luxia::Components::MeshRenderer>()) {
				if (ImGui::Button("Mesh Renderer Component", button_size)) {
					ent.transform->AddComponent<Luxia::Components::MeshRenderer>();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
	void InspectorPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void InspectorPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}