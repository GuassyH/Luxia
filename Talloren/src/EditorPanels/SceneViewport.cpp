#include "SceneViewport.h"
#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Talloren::Panels {
	void SceneViewport::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - SceneView Panel: Init");

		entt::entity ent = editorLayer->editor_reg.create();
		editorLayer->editor_reg.emplace<Luxia::Components::Transform>(ent);
		cam_ent = editorLayer->editor_reg.try_get<Luxia::Components::Transform>(ent);
		cam_ent->transform = cam_ent;
		cam_ent->reg = &editorLayer->editor_reg;

		cam_ent->AddComponent<Luxia::Components::Camera>(1920, 1080);
		cam_ent->AddComponent<Talloren::Scripts::SceneCameraScript>();
	}

	void SceneViewport::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Scene View", (bool*)0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		Luxia::Components::Camera& cam = cam_ent->GetComponent<Luxia::Components::Camera>();
		Talloren::Scripts::SceneCameraScript& cam_script = cam_ent->GetComponent<Talloren::Scripts::SceneCameraScript>();

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
			if (Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_2)) {
				if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_2)) {
					cam_script.last_mouseX = Luxia::Input::GetMousePosition().x;
					cam_script.last_mouseY = Luxia::Input::GetMousePosition().y;
				}

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
				cam_script.Look();
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
	void SceneViewport::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
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