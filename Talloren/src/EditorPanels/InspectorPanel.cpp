#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panels {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}

	std::string InspectorPanel::PasteFromClipboard()
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
	void InspectorPanel::RenderEntity(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, Luxia::GUID e_guid) {
		ImGui::BeginChild("Entity");

		Luxia::Entity& ent = scene->runtime_entities.find(e_guid)->second;

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
			editorLayer->EraseSelected(ent.guid);
			scene->DeleteEntity(ent.guid);
			ImGui::End();
			return;
		}

		// GUID
		std::ostringstream info; info << "Entity ID: " << (uint64_t)ent.guid;
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

						// MESH
						DrawPasteField<Luxia::Mesh>(editorLayer, meshrend->mesh, "Mesh");

						// MATERIAL
						DrawPasteField<Luxia::IMaterial>(editorLayer, meshrend->material, "Material");

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
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + (windowSize.x - popupWidth) / 2.0f, ImGui::GetWindowPos().y + (windowSize.y - popupHeight) / 2.0f));
		if (ImGui::BeginPopup("Add Component", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("Add Component");
			ImGui::Separator();

			if (ent.transform) {
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

		ImGui::EndChild();
	}

	void InspectorPanel::RenderMaterial(Talloren::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
		ImGui::BeginChild("Material");

		std::shared_ptr<Luxia::IMaterial> mat = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(guid);
		if (!mat) { return; }

		ImGui::Text("Material: %s", mat->name.c_str());
		ImGui::Separator();

		DrawPasteField<Luxia::IShader>(editorLayer, mat->shader, "Shader");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// Diffuse Texture
		DrawPasteField<Luxia::ITexture>(editorLayer, mat->diffuse_texture, "Diffuse");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
		ImGui::Image(mat->diffuse_texture ? (void*)(uintptr_t)mat->diffuse_texture->texID : nullptr, ImVec2(100, 100)); // should draw "no assigned tex@

		// Specular Texture
		DrawPasteField<Luxia::ITexture>(editorLayer, mat->specular_texture, "Specular");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
		ImGui::Image(mat->specular_texture ? (void*)(uintptr_t)mat->specular_texture->texID : nullptr, ImVec2(100, 100)); // should draw "no assigned tex@
		
		// Normal Texture
		DrawPasteField<Luxia::ITexture>(editorLayer, mat->normal_texture, "Normals");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100);
		ImGui::Image(mat->normal_texture ? (void*)(uintptr_t)mat->normal_texture->texID : nullptr, ImVec2(100, 100)); // should draw "no assigned tex@
		

		if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit4("Color", &mat->color.r);
			ImGui::SliderFloat("Metallic", &mat->metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("Roughness", &mat->roughness, 0.0f, 1.0f);
		}

		ImGui::EndChild();
	}

	void InspectorPanel::RenderMesh(Talloren::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
		ImGui::BeginChild("Mesh");
		
		std::shared_ptr<Luxia::Mesh> mesh = editorLayer->GetAssetManager()->GetAsset<Luxia::Mesh>(guid);
		if (!mesh) { return; }

		ImGui::Text("Mesh: %s", mesh->name.c_str());
		ImGui::Separator();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		std::ostringstream vertinfo; vertinfo << "Vertices: " << mesh->vertices.size();
		ImGui::PushID("MeshVerts");
		bool v_open = ImGui::TreeNodeEx(vertinfo.str().c_str(), flags);
		ImGui::PopID();

		if (v_open) {
			std::error_code ec;

			for (const Luxia::Rendering::Vertex& vert : mesh->vertices) {
				if (ec) continue;
				ImGui::Text("Position: [%.2f, %.2f, %.2f]", vert.pos.x, vert.pos.y, vert.pos.z);
			}
			ImGui::TreePop();
		}

		std::ostringstream indinfo; indinfo << "Indices: " << mesh->indices.size();
		ImGui::PushID("MeshInds");
		bool i_open = ImGui::TreeNodeEx(indinfo.str().c_str(), flags);
		ImGui::PopID();

		if (i_open) {
			for (int i = 0; i < mesh->indices.size(); i++) {
				ImGui::Text("[%u]", mesh->indices[i]); // uint32_t
				if (i % 3 == 2)
					ImGui::Separator();
				else
					ImGui::SameLine();
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();
	}

	void InspectorPanel::RenderShader(Talloren::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
		ImGui::BeginChild("Shader");

		std::shared_ptr<Luxia::IShader> shader = editorLayer->GetAssetManager()->GetAsset<Luxia::IShader>(guid);
		if (!shader) { return; }

		ImGui::Text("Shader: %s", shader->name.c_str());
		ImGui::Separator();

		std::string typetext = "Type: Surface";
		ImGui::Text(typetext.c_str());
		if(ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "Currently, only surface shaders are supported.");
			ImGui::EndTooltip();
		}

		ImGui::Spacing();
		ImGui::TextWrapped("Vertex Shader Path: %s", shader->GetVertPath().c_str());
		ImGui::TextWrapped("Fragment Shader Path: %s", shader->GetFragPath().c_str());

		ImGui::EndChild();
	}


	static enum class InspectorMode {
		Entity,
		Material,
		Mesh,
		Texture,
		Shader,
		None
	};
	InspectorMode currentMode = InspectorMode::None;

	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		currentMode = InspectorMode::None;

		if (editorLayer->areNoneSelected) { ImGui::End(); return; }
		if (!editorLayer->isOneSelected) { 
			ImGui::Text("Multiple items selected - Not supported yet");
			ImGui::End(); 
			return; 
		}
		
		Luxia::GUID e_guid = Luxia::GUID(0);
		e_guid = *editorLayer->selected_assets.begin();

		bool foundtype = false;
		if (scene) {
			if (scene->runtime_entities.contains(e_guid)) {
				currentMode = InspectorMode::Entity;
				foundtype = true;
			}
		}

		if (!foundtype) {
			if (!editorLayer->GetAssetManager()->GetAssetPool().contains(e_guid))
				currentMode = InspectorMode::None;
			else if (editorLayer->GetAssetManager()->HasAsset<Luxia::Mesh>(e_guid))
				currentMode = InspectorMode::Mesh;
			else if (editorLayer->GetAssetManager()->HasAsset<Luxia::IMaterial>(e_guid))
				currentMode = InspectorMode::Material;
			else if (editorLayer->GetAssetManager()->HasAsset<Luxia::ITexture>(e_guid))
				currentMode = InspectorMode::Texture;
			else if (editorLayer->GetAssetManager()->HasAsset<Luxia::IShader>(e_guid))
				currentMode = InspectorMode::Shader;
		}


		switch (currentMode)
		{
		case Talloren::Panels::InspectorMode::Entity:
			RenderEntity(editorLayer, scene, e_guid);
			break;
		case Talloren::Panels::InspectorMode::Material:
			RenderMaterial(editorLayer, e_guid);
			break;
		case Talloren::Panels::InspectorMode::Mesh:
			RenderMesh(editorLayer, e_guid);
			break;
		case Talloren::Panels::InspectorMode::Shader:
			RenderShader(editorLayer, e_guid);
			break;
		default:
			break;
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