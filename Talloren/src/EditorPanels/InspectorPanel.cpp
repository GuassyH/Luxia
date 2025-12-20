#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}

	static std::string PasteFromClipboard()
	{
		std::string result;

		if (!OpenClipboard(nullptr))
			return result;

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData)
		{
			char* pszText = static_cast<char*>(GlobalLock(hData));
			if (pszText)
			{
				result = pszText;
				GlobalUnlock(hData);
			}
		}

		CloseClipboard();
		return result;
	}

	static char namebuff[255] = {};
	static char meshbuff[255] = {};
	static char matbuff[255] = {};
	void RenderEntity(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		if (!editorLayer->is_entity_selected) { return; }
		if (!scene->runtime_entities.contains(editorLayer->selected_entity)) { return; }

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
		if (!ent.transform) { return; }

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ent.transform->OnInspectorDraw();
		}

		// Loop through all components and render them
		for (auto& comp : Luxia::componentRegistry) {
			if (comp.hasFunc(ent.transform)) {
				if (comp.name == "Mesh Renderer") {
					// Needs to be here to use asset_manager
					auto meshrend = ent.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
					if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
						meshrend->OnInspectorDraw();

						// This needs to be here due to asset manager usage 

						int flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll |
							ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_ElideLeft;

						// MESH
						if (ImGui::Button("Paste##MeshGUIDPaste")) {
							std::string paste = PasteFromClipboard();
							Luxia::GUID meshguid(std::strtoull(paste.c_str(), nullptr, 10));
							if (editorLayer->GetAssetManager()->HasAsset<Luxia::Mesh>(meshguid))
								meshrend->mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(meshguid);
							else
								LX_ERROR("Tried to assign asset that is not mesh or has invalid guid: {}", (uint64_t)meshguid);
						}

						std::ostringstream mesht; mesht << "Mesh: ";
						if (meshrend->mesh) 
							mesht << meshrend->mesh->name;
						else 
							mesht << "nullptr";
						std::string meshLabel = mesht.str();
						std::string meshHint = meshrend->mesh ? std::to_string(meshrend->mesh->guid) : "none";

						ImGui::SameLine();
						ImGui::Text(meshLabel.c_str());
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::Text("Mesh GUID");
							ImGui::Separator();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), meshHint.c_str());
							ImGui::EndTooltip();
						}

						// MATERIAL
						if (ImGui::Button("Paste##MatGUIDPaste")) {
							std::string paste = PasteFromClipboard();
							Luxia::GUID matguid(std::strtoull(paste.c_str(), nullptr, 10));
							if (editorLayer->GetAssetManager()->HasAsset<Luxia::IMaterial>(matguid))
								meshrend->material = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(matguid);
							else
								LX_ERROR("Tried to assign asset that is not material or has invalid guid: {}", (uint64_t)matguid);
						}

						std::ostringstream matt; matt << "Material: ";
						if (meshrend->material) 
							matt << meshrend->material->name;
						else 
							matt << "nullptr";

						std::string matLabel = matt.str(); 
						std::string matHint = meshrend->material ? std::to_string(meshrend->material->guid) : "none";

						ImGui::SameLine();
						ImGui::Text(matLabel.c_str());
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::Text("Material GUID");
							ImGui::Separator();
							ImGui::TextColored(ImVec4(1, 1, 0, 1), matHint.c_str());
							ImGui::EndTooltip();
						}
					}
				}

				// Draw Regular
				else if (ImGui::CollapsingHeader(comp.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					comp.getFunc(ent.transform)->OnInspectorDraw();
				}
				
			}
		}

		// TEMP ADD COMPONENT
		float avail = ImGui::GetContentRegionAvail().x;
		float buttonWidth = 120.0f; // make it big enough
		ImGui::SetCursorPosX((avail - buttonWidth) / 2.0f);
		if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0))) {
			ImGui::OpenPopup("Add Component");
		}

		// should be menu
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 contentSize = ImGui::CalcTextSize("Add Component");
		float popupWidth = 200.0f;
		float popupHeight = contentSize.y * Luxia::componentRegistry.size() + 50; // estimate

		ImGui::SetNextWindowSize(ImVec2(popupWidth, popupHeight), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + (windowSize.x - popupWidth) / 2.0f,ImGui::GetWindowPos().y + (windowSize.y - popupHeight) / 2.0f));
		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Add Component");
			ImGui::Separator();

			if (ent.transform){
				for (auto& comp : Luxia::componentRegistry) {
					if (!comp.hasFunc(ent.transform)) {
						if (ImGui::MenuItem(comp.name.c_str())) {
							comp.addFunc(ent.transform);
							ImGui::CloseCurrentPopup();
						}
					}
				}
			}
			ImGui::EndPopup();
		}
	}


	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel");


		// ENTITY STUFF
		// if (editorLayer->SelectedIsTypeEntity)
		RenderEntity(editorLayer, scene);

		ImGui::End();
	}
	void InspectorPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void InspectorPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}