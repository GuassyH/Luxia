#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}

	char buff[255];
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
		if (ImGui::InputTextWithHint(" ", entname.str().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
			ent.name = buff;
			memset(buff, 0, 255);
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
		auto meshrend = ent.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
		if (meshrend) {
			if (ImGui::CollapsingHeader(meshrend->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				meshrend->OnInspectorDraw();
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