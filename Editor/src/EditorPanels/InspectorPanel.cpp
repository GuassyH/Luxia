#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Editor::Panels {
	void InspectorPanel::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}


	
	static char namebuff[255] = {};
	static char meshbuff[255] = {};
	static char matbuff[255] = {};
	void InspectorPanel::RenderEntity(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, Luxia::GUID e_guid) {
		std::string componentToRemove = "none";

		ImGui::BeginChild("Entity");
		Luxia::Entity& ent = scene->runtime_entities.find(e_guid)->second;

		// Name
		std::ostringstream entname; entname << ent.name;

		ImGui::Checkbox("##IsEnabled", &ent.transform->enabled);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - ImGui::CalcTextSize(entname.str().c_str()).x * 0.5f);
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 180 * 0.5f);
		ImGui::SetNextItemWidth(180);
		if (ImGui::InputTextWithHint("##NameInput", entname.str().c_str(), namebuff, sizeof(namebuff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll)) {
			ent.name = namebuff;
			memset(namebuff, 0, sizeof(namebuff));
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
		bool drawnCollider = false;
		for (auto& comp : Luxia::componentRegistry) {
			if (comp.hasFunc(ent.transform)) {
				// Incase its a collider, we only want to draw one of them, since they share the same component
				if (comp.name == "Box Collider" || comp.name == "Sphere Collider") {
					if (drawnCollider)
						goto SkipDraw;
					else 
						drawnCollider = true;
				}

				if (ImGui::CollapsingHeader(comp.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					// For removing
					if (ImGui::BeginPopupContextItem((std::string("CompContext_") + comp.name).c_str())) {
						if (ImGui::MenuItem("Remove"))
							componentToRemove = comp.name;
						ImGui::EndPopup();
					}


					comp.getFunc(ent.transform)->OnInspectorDraw();
				
					// Special Cases for components that need to use editor layer or other external functionality
					if (comp.name == "Mesh Renderer") {
						auto meshrend = ent.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
						DrawDropField<Luxia::Mesh>(editorLayer, meshrend->mesh, "Mesh");
						DrawDropField<Luxia::IMaterial>(editorLayer, meshrend->material, "Material");
					}
					
					// In case we skip
				SkipDraw:
					continue;

				}
			}
		}

		// Add Component Popup
		float avail = ImGui::GetContentRegionAvail().x;
		float buttonWidth = 120.0f; // make it big enough
		ImGui::SetCursorPosX((avail - buttonWidth) / 2.0f);
		if (ImGui::Button("Add Component", ImVec2(buttonWidth, 0))) {
			ImGui::OpenPopup("Add Component");
		}

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

		// Remove Component functionality
		if (componentToRemove != "none") {
			if (componentToRemove == "Transform") { LX_ERROR("Cannot Remove Transform Component - Functionality not implemented yet"); }
			else {
				if (componentToRemove == "Box Collider" || componentToRemove == "Sphere Collider") {
					ent.transform->RemoveComponent<Luxia::Components::Collider>();
				}
				else {
					for (auto& comp : Luxia::componentRegistry) {
						if (comp.name == componentToRemove) {
							comp.removeFunc(ent.transform);
							break;
						}
					}
				}
			}
			componentToRemove = "none";
		}


		ImGui::EndChild();
	}

	void InspectorPanel::RenderMaterial(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
		ImGui::BeginChild("Material");

		std::shared_ptr<Luxia::IMaterial> mat = editorLayer->GetAssetManager()->GetAsset<Luxia::IMaterial>(guid);
		if (!mat) { return; }

		ImGui::Text("Material: %s", mat->name.c_str());
		ImGui::Separator();

		DrawDropField<Luxia::IShader>(editorLayer, mat->shader, "Shader");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// Diffuse Texture
		DrawDropField<Luxia::ITexture>(editorLayer, mat->diffuse_texture, "Diffuse");
		ImGui::SameLine();
		float av = ImGui::GetContentRegionAvail().x; 
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - av);
		ImGui::Image(mat->diffuse_texture ? (ImTextureRef)mat->diffuse_texture->texID : editorLayer->NoImageTex->texID, ImVec2(av, av)); // should draw "no assigned tex@

		// Specular Texture
		DrawDropField<Luxia::ITexture>(editorLayer, mat->specular_texture, "Specular");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - av);
		ImGui::Image(mat->specular_texture ? (ImTextureRef)mat->specular_texture->texID : editorLayer->NoImageTex->texID, ImVec2(av, av)); // should draw "no assigned tex@
		
		// Normal Texture
		DrawDropField<Luxia::ITexture>(editorLayer, mat->normal_texture, "Normals");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - av);
		ImGui::Image(mat->normal_texture ? (ImTextureRef)mat->normal_texture->texID : editorLayer->NoImageTex->texID, ImVec2(av, av)); // should draw "no assigned tex@
		
		if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit4("Color", &mat->color.r);
			ImGui::SliderFloat("Metallic", &mat->metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("Roughness", &mat->roughness, 0.0f, 1.0f);
		}

		ImGui::EndChild();
	}

	void InspectorPanel::RenderMesh(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
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

	void InspectorPanel::RenderShader(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid) {
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


	enum class InspectorMode {
		Entity,
		Material,
		Mesh,
		Texture,
		Shader,
		None
	};
	InspectorMode currentMode = InspectorMode::None;

	void InspectorPanel::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Inspector Panel";
		TabPopupContextMenu();

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
		case Editor::Panels::InspectorMode::Entity:
			RenderEntity(editorLayer, scene, e_guid);
			break;
		case Editor::Panels::InspectorMode::Material:
			RenderMaterial(editorLayer, e_guid);
			break;
		case Editor::Panels::InspectorMode::Mesh:
			RenderMesh(editorLayer, e_guid);
			break;
		case Editor::Panels::InspectorMode::Shader:
			RenderShader(editorLayer, e_guid);
			break;
		default:
			break;
		}



		ImGui::End();
	}
	void InspectorPanel::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void InspectorPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}

}