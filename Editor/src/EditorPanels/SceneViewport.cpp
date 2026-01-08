#include "SceneViewport.h"
#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Editor::Panels {

	static std::shared_ptr<Luxia::ITexture> fbo_pick_tex = Luxia::Platform::Assets::CreateTexture();

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
			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
				glm::vec2 rectp = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
				glm::vec2 rects = glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
				glm::vec2 rp = Luxia::Screen::GetMousePosRect(rectp, rects, Luxia::Input::GetMousePosition());

				if (cam.width != fbo_pick_tex->GetWidth() || cam.height != fbo_pick_tex->GetHeight()) {
					fbo_pick_tex->Delete();
					fbo_pick_tex->CreateFBOTex(cam.width, cam.height);
				}

				Luxia::GUID picked = Luxia::Screen::GetMousePosEntity(rp, &cam, scene, editorLayer->GetRenderer(), fbo_pick_tex);

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