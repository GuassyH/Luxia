#include "SceneViewport.h"
#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Editor::Panels {

	static std::shared_ptr<Luxia::ITexture> fbo_pick_tex = Luxia::Platform::Assets::CreateTexture();

	static Luxia::GUID GetMousePosEntity(glm::vec2 mouse_pos, Luxia::Components::Camera* cam, const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> renderer, std::shared_ptr<Luxia::ITexture> output_texture) {

		if (!output_texture->IsValid() || !output_texture->IsFBOTex()) return Luxia::GUID(0);

		Luxia::GUID result = Luxia::GUID(0);

		if (cam->width != output_texture->GetWidth() || cam->height != output_texture->GetHeight()) {
			output_texture->Delete();
			output_texture->CreateFBOTex(cam->width, cam->height);
		}

		Luxia::Screen::BindFBO(output_texture->GetFBO());
		Luxia::Screen::BindRBO(output_texture->GetRBO());
		
		/// OPENGL DEPENDANT
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, cam->width, cam->height);

		// Creates a copy of the unlit mat (so we can change color no problem)
		std::shared_ptr<Luxia::IMaterial> id_mat = Luxia::Platform::Assets::CreateMaterial();
		id_mat->shader = Luxia::ResourceManager::DefaultUnlitMaterial->shader;
		glm::vec4 id_color = glm::vec4(0.0f);

		auto mesh_view = scene->GetEntitiesWith<Luxia::Components::MeshRenderer>();

		for (auto entity : mesh_view) {
			auto& mr = scene->GetFromEntity<Luxia::Components::MeshRenderer>(entity);

			if (mr.transform) {
				int id = (int)mr.transform->ent_id;
				id_color.r = ((id & 0x000000FF) >> 0) / 255.0f;
				id_color.g = ((id & 0x0000FF00) >> 8) / 255.0f;
				id_color.b = ((id & 0x00FF0000) >> 16) / 255.0f;
				id_color.a = 1.0f;
				id_mat->color = id_color;

				renderer->RenderMesh(mr.mesh.get(), id_mat.get(), mr.transform->GetMatrix(),
					cam->GetCamera()->GetViewMat(), cam->GetCamera()->GetProjMat());
			}
		}

		/// OPENGL DEPENDANT
		GLubyte pixel[4];
		glReadPixels(static_cast<GLint>(mouse_pos.x), static_cast<GLint>(mouse_pos.y), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

		// if pixel is white (255, 255, 255) then its the LAST ID aka no entity (unless you have 16777215 (255^3) entities) 
		if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
			// Do sum?
		}
		else {
			unsigned int pickedID = (static_cast<unsigned int>(pixel[0]) << 0) | (static_cast<unsigned int>(pixel[1]) << 8) | (static_cast<unsigned int>(pixel[2]) << 16);
			result = scene->GetFromEntity<Luxia::Components::Transform>(entt::entity(pickedID)).ent_guid;
		}

		Luxia::Screen::BindFBO(0);
		Luxia::Screen::BindRBO(0);
		output_texture->Unbind();

		return result;
	}


	void SceneViewport::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - SceneView Panel: Init");

		entt::entity ent = editorLayer->editor_reg.create();
		editorLayer->editor_reg.emplace<Luxia::Components::Transform>(ent);
		cam_ent = editorLayer->editor_reg.try_get<Luxia::Components::Transform>(ent);
		cam_ent->transform = cam_ent;
		cam_ent->reg = &editorLayer->editor_reg;

		cam_ent->AddComponent<Luxia::Components::Camera>(1920, 1080).farPlane = 2000.0f;
		cam_ent->AddComponent<Editor::Scripts::SceneCameraScript>();

		fbo_pick_tex->CreateFBOTex(1920, 1080);
	}

	void SceneViewport::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		name = "Scene View";
		TabPopupContextMenu();

		Luxia::Components::Camera& cam = cam_ent->GetComponent<Luxia::Components::Camera>();
		Editor::Scripts::SceneCameraScript& cam_script = cam_ent->GetComponent<Editor::Scripts::SceneCameraScript>();

		// Set the image
		if (output_texture) { 
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			// Camera aspect ratio (width / height)

			ImVec2 imageSize = ImGui::GetWindowSize();	imageSize.y -= 20; // Some sort of padding

			ImVec2 imagePosInWindow;
			imagePosInWindow.x = ((windowSize.x - imageSize.x) * 0.5f) + ImGui::GetCursorPosX();
			imagePosInWindow.y = ((windowSize.y - imageSize.y) * 0.5f) + ImGui::GetCursorPosY();

			ImGui::SetCursorPosX(imagePosInWindow.x);
			ImGui::SetCursorPosY(imagePosInWindow.y);

			ImGui::Image((ImTextureID)(intptr_t)output_texture->texID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
		}

		// Render last, so that the next frame instantly displays the render output
		cam.width = ImGui::GetWindowWidth();
		cam.height = ImGui::GetWindowHeight() - 20;  // Some sort of padding

		if (ImGui::IsWindowHovered()) {

			// Picking
			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
				glm::vec2 rectp = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
				glm::vec2 rects = glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				glm::vec2 rp = Luxia::Screen::GetMousePosRect(rectp, rects, glm::vec2(cam.width, cam.height), Luxia::Input::GetMousePosition());

				Luxia::GUID picked = GetMousePosEntity(rp, &cam, scene, editorLayer->GetRenderer(), fbo_pick_tex);

				if (scene->runtime_entities.contains(picked)) {
					if (Luxia::Input::IsKeyPressed(LX_KEY_LEFT_SHIFT)) {
						editorLayer->InsertSelected(picked);
					}
					else {
						editorLayer->ClearSelected();
						editorLayer->InsertSelected(picked);
					}
				}
				else {
					editorLayer->ClearSelected();
				}
			}

			// Movement
			if (Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_2)) {
				if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_2)) {
					cam_script.last_mouseX = Luxia::Input::GetMousePosition().x;
					cam_script.last_mouseY = Luxia::Input::GetMousePosition().y;
				}

				cam_script.Look();
				if(editorLayer->isOneSelected) {
					Luxia::GUID selected_guid = *editorLayer->selected_assets.begin();
					auto it = scene->runtime_entities.find(selected_guid);
					if (it != scene->runtime_entities.end()) {
						Luxia::Components::Transform* focused_t = scene->TryGetFromEntity<Luxia::Components::Transform>(it->second);
						cam_script.Move(focused_t);
					}
					else {
						cam_script.Move(nullptr);
					}
				}
				else {
					cam_script.Move(nullptr);
				}
			}
		}

		ImGui::End();

		if (scene) {
			// Render Camera
			std::shared_ptr<Luxia::ITexture> tex = cam.Render(scene, editorLayer->GetRenderer());
			// Render Gizmos?
			editorLayer->GetEventHandler().PushEvent(std::make_shared<Luxia::RenderCameraEvent>(tex, false, true));
		}
		
	}
	void SceneViewport::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		// Remove component
		cam_ent->RemoveComponent<Luxia::Components::Camera>();
		editorLayer->editor_reg.remove<Luxia::Components::Transform>(cam_ent->ent_id);
		
		cam_ent = nullptr;
		output_texture = nullptr;
	}

	bool SceneViewport::RenderImage(Luxia::RenderCameraEvent& e) {
		if (!e.GetTexture()) { return false; }
		if (!e.IsEditorCamera()) { return false; }

		output_texture = e.GetTexture();

		return true; // Consume
	}

	void SceneViewport::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}


}