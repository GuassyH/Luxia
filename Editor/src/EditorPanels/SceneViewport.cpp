#include "SceneViewport.h"
#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Editor::Panels {

	static std::shared_ptr<Luxia::ITexture> fbo_pick_tex = Luxia::Platform::Assets::CreateTexture();
	static std::shared_ptr<Luxia::ITexture> selection_fbo = Luxia::Platform::Assets::CreateTexture();
	static std::shared_ptr<Luxia::IMaterial> outline_mat = Luxia::Platform::Assets::CreateMaterial();
	static std::shared_ptr<Luxia::IShader> outline_shader = nullptr;

	static std::shared_ptr<Gizmos::GridGizmo> grid_gizmo = nullptr;

	enum EditType {
		Translate = 0,
		Rotate = 1,
		Scale = 2
	};
	static EditType editType = EditType::Translate;

	static Luxia::GUID GetMousePosEntity(glm::vec2 mouse_pos, Luxia::Components::Camera* cam, const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> renderer, std::shared_ptr<Luxia::ITexture> output_texture) {

		if (!output_texture->IsValid() || !output_texture->IsFBOTex()) return Luxia::GUID(0);

		Luxia::GUID result = Luxia::GUID(0);

		if (cam->width != output_texture->GetWidth() || cam->height != output_texture->GetHeight()) {
			output_texture->Unload();
			output_texture->CreateFBOTex(cam->width, cam->height);
		}

		Luxia::Screen::BindFBO(output_texture->GetFBO());
		Luxia::Screen::BindRBO(output_texture->GetRBO());
		
		// OPENGL DEPENDANT
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

			if (mr.enabled && mr.transform) {
				int id = static_cast<int>(mr.transform->ent_id);
				id_color.r = ((id & 0x000000FF) >> 0) / 255.0f;
				id_color.g = ((id & 0x0000FF00) >> 8) / 255.0f;
				id_color.b = ((id & 0x00FF0000) >> 16) / 255.0f;
				id_color.a = 1.0f;
				id_mat->color = id_color;

				renderer->RenderMesh(mr.mesh.get(), id_mat.get(), mr.transform->GetMatrix(),
					cam->GetCamera()->GetViewMat(), cam->GetCamera()->GetProjMat());
			}
		}

		// OPENGL DEPENDANT
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


	/// CORE
	void SceneViewport::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - SceneView Panel: Init");

		entt::entity ent = editorLayer->editor_reg.create();
		editorLayer->editor_reg.emplace<Luxia::Components::Transform>(ent);
		cam_ent = editorLayer->editor_reg.try_get<Luxia::Components::Transform>(ent);
		cam_ent->transform = cam_ent;
		cam_ent->reg = &editorLayer->editor_reg;
		cam_ent->transform->UpdateMatrix();

		cam_ent->AddComponent<Luxia::Components::Camera>(1920, 1080).farPlane = 2000.0f;
		cam_ent->AddComponent<Editor::Scripts::SceneCameraScript>();


		fbo_pick_tex->CreateFBOTex(1920, 1080);
		selection_fbo->CreateDepthTex(1920, 1080);

		outline_shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/Editor/resources/gizmos/outline.frag", "C:/dev/Luxia/Editor/resources/gizmos/outline.vert");

		outline_mat->shader = outline_shader;
		outline_mat->guid = Luxia::GUID();
		outline_mat->color = glm::vec4(1.0f, 0.55f, 0.2f, 1.0f);


		InitGizmos(editorLayer);
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
		cam.width = static_cast<int>(ImGui::GetWindowWidth());
		cam.height = static_cast<int>(ImGui::GetWindowHeight() - 20);  // Some sort of padding

		if (ImGui::IsWindowHovered()) {

			// Picking
			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
				if (scene) {
					glm::vec2 rp = Luxia::Screen::GetMousePosRect(glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y), glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), glm::vec2(cam.width, cam.height), Luxia::Input::GetMousePosition());
					Luxia::GUID picked = GetMousePosEntity(rp, &cam, scene, editorLayer->GetRenderer(), fbo_pick_tex);

					if (scene->runtime_entities.contains(picked)) {
						if (Luxia::Input::IsKeyPressed(LX_KEY_LEFT_CONTROL)) {
							if(!editorLayer->selected_assets.contains(picked))
								editorLayer->InsertSelected(picked);
							else
								editorLayer->EraseSelected(picked);
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
			}

			// Movement
			Luxia::Components::Transform* focused_t = nullptr;
			if(editorLayer->isOneSelected) {
				Luxia::GUID selected_guid = *editorLayer->selected_assets.begin();
				auto it = scene->runtime_entities.find(selected_guid);
				if (it != scene->runtime_entities.end()) {
					focused_t = scene->TryGetFromEntity<Luxia::Components::Transform>(it->second);
				}
			}
		
			if (Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_2)) {
				if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_2)) {
					cam_script.last_mouseX = Luxia::Input::GetMousePosition().x;
					cam_script.last_mouseY = Luxia::Input::GetMousePosition().y;
				}

				cam_script.Look();
				cam_script.Move();
			}
			else {
				if (!Luxia::Input::IsKeyPressed(LX_KEY_LEFT_CONTROL)) {
					if (Luxia::Input::IsKeyJustPressed(LX_KEY_W))
						editType = EditType::Translate;
					else if (Luxia::Input::IsKeyJustPressed(LX_KEY_S))
						editType = EditType::Scale;
					else if (Luxia::Input::IsKeyJustPressed(LX_KEY_R))
						editType = EditType::Rotate;
				}
			}

			if (Luxia::Input::IsKeyJustPressed(LX_KEY_F)) {
				if (focused_t) {
					cam.transform->local_position = focused_t->local_position + (cam.transform->forward * -10.0f);
				}
			}
		}

		ImGui::End();

		if (scene) {
			// Render
			std::shared_ptr<Luxia::ITexture> tex = cam.Render(scene, editorLayer->GetRenderer());
			RenderGizmos(editorLayer, scene.get(), tex);
			// Push for next frame
			editorLayer->GetEventHandler().PushEvent(std::make_shared<Luxia::RenderCameraEvent>(tex, false, true));
		}
		
	}
	void SceneViewport::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		// Remove component
		cam_ent->RemoveComponent<Luxia::Components::Camera>();
		editorLayer->editor_reg.remove<Luxia::Components::Camera>(cam_ent->ent_id);
		editorLayer->editor_reg.remove<Luxia::Components::Transform>(cam_ent->ent_id);
		
		cam_ent = nullptr;
		output_texture = nullptr;

		fbo_pick_tex->Unload();
		selection_fbo->Unload();
		outline_shader->Unload();
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

	
	/// GIZMOS
	void SceneViewport::InitGizmos(Editor::Layers::EditorLayer* editorLayer) {
		Gizmos::GizmoResources::Init();
		gizmos.push_back(std::make_unique<Gizmos::TranslateGizmo>(editorLayer->editor_reg, std::filesystem::path("C:/dev/Luxia/Editor/resources/gizmos")));
		gizmos.push_back(std::make_unique<Gizmos::RotateGizmo>(editorLayer->editor_reg, std::filesystem::path("C:/dev/Luxia/Editor/resources/gizmos")));
		gizmos.push_back(std::make_unique<Gizmos::ScaleGizmo>(editorLayer->editor_reg, std::filesystem::path("C:/dev/Luxia/Editor/resources/gizmos")));
		grid_gizmo = std::make_shared<Gizmos::GridGizmo>(editorLayer->editor_reg, std::filesystem::path("C:/dev/Luxia/Editor/resources/gizmos"));
	}

	void SceneViewport::RenderGizmos(Editor::Layers::EditorLayer* editorLayer, Luxia::Scene* scene, std::shared_ptr<Luxia::ITexture> cam_tex) {
		// Get the important stuff
		Luxia::Rendering::IRenderer* renderer = editorLayer->GetRenderer().get();
		cam_ent->UpdateMatrix();
		auto& cam = cam_ent->GetComponent<Luxia::Components::Camera>();
		
		// Rebind
		Luxia::Screen::BindFBO(cam_tex->GetFBO());
		Luxia::Screen::BindRBO(cam_tex->GetRBO());

		if (cam.width != selection_fbo->GetWidth() || cam.height != selection_fbo->GetHeight()) {
			selection_fbo->Unload();
			selection_fbo->CreateDepthTex(cam.width, cam.height);
		}


		/// With Depth

		/* Grid
		auto& mr = grid_gizmo->gizmo_parts[0]->transform->GetComponent<Luxia::Components::MeshRenderer>();
		mr.transform->local_position = cam.transform->local_position;
		mr.transform->local_position.y = 0.0f;
		mr.transform->UpdateMatrix();
		mr.material->Use();
		mr.material->shader->SetMat4("modelMat", mr.transform->GetMatrix());
		mr.material->shader->SetMat4("viewMat", cam.GetCamera()->GetViewMat());
		mr.material->shader->SetMat4("projMat", cam.GetCamera()->GetProjMat());
		mr.material->shader->SetVec3("camPos", cam.transform->local_position);
		renderer->RenderMeshPure(*mr.mesh.get());
		*/

		// OUTLINE
		if (!editorLayer->areNoneSelected) {
			bool is_one_ent = false;
			for (auto& guid : editorLayer->selected_assets) {
				if (scene->runtime_entities.contains(guid)) {
					if (scene->runtime_entities[guid].transform->HasComponent<Luxia::Components::MeshRenderer>()) {
						is_one_ent = true;
					}
				}
			}

			if (is_one_ent) {
				// Bind Depth only stuff
				Luxia::Screen::BindFBO(selection_fbo->GetFBO());
				glViewport(0, 0, selection_fbo->GetWidth(), selection_fbo->GetHeight());
				Luxia::Screen::BindRBO(0);

				glClear(GL_DEPTH_BUFFER_BIT);
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				glEnable(GL_DEPTH_TEST);

				for (auto& guid : editorLayer->selected_assets) {
					if (scene->runtime_entities.contains(guid)) {
						auto& ent = scene->runtime_entities.find(guid)->second;
						auto mr = ent.transform->TryGetComponent<Luxia::Components::MeshRenderer>();
						if (mr) 
							renderer->RenderMesh(mr->mesh.get(), Luxia::ResourceManager::DepthOnlyMaterial.get(), ent.transform->GetMatrix(), cam.GetCamera()->GetViewMat(), cam.GetCamera()->GetProjMat());
					}
				}
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					
				// Selection Pass
				Luxia::Screen::BindFBO(cam_tex->GetFBO());
				Luxia::Screen::BindRBO(cam_tex->GetRBO());

				glDisable(GL_DEPTH_TEST);

				// Render outline using the output_texture FBO
				outline_mat->Use();
					
				glActiveTexture(GL_TEXTURE0);
				selection_fbo->Bind();
				outline_mat->shader->SetInt("depthSample", 0);

				glActiveTexture(GL_TEXTURE1);
				cam_tex->Bind();
				outline_mat->shader->SetInt("baseSample", 1);

				outline_mat->shader->SetVec2("texelSize", glm::vec2(1.0f / static_cast<float>(cam_tex->GetWidth()), 1.0f / static_cast<float>(cam_tex->GetHeight())));
				outline_mat->shader->SetFloat("farPlane", cam.farPlane);
				outline_mat->shader->SetFloat("nearPlane", cam.nearPlane);
				outline_mat->shader->SetFloat("depthThreshold", 1.0f);

				renderer->RenderMeshPure(*Luxia::ResourceManager::DefaultQuad.get());

				glEnable(GL_DEPTH_TEST);
				glActiveTexture(GL_TEXTURE0);
			}
		}

		/// Without Depth
		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw translation gizmos
		if (editorLayer->isOneSelected) {
			if (scene->runtime_entities.contains(*editorLayer->selected_assets.begin())) {
				auto& ent = scene->runtime_entities.find(*editorLayer->selected_assets.begin())->second;

				glm::vec3 cam_to_entity = cam.transform->world_position + (glm::normalize(ent.transform->world_position - cam.transform->world_position) * 15.0f);
				glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), cam_to_entity);
				
				glm::quat entityRot = ent.transform->local_rotation;

				for (auto part : gizmos[editType].get()->gizmo_parts) {
					if (part->transform) {
						auto part_mr = part->transform->TryGetComponent<Luxia::Components::MeshRenderer>();
						if (!part_mr) continue;
						part->transform->UpdateMatrix();

						glm::quat gizmoRot = part->transform->local_rotation;
						glm::quat rotationQuat = entityRot * gizmoRot;
						glm::mat4 rotationMatrix = glm::mat4(1.0f);

						if (ent.transform->HasParent())
							rotationMatrix *= glm::mat4(ent.transform->parent->GetRotationMatrix());

						rotationMatrix *= glm::mat4_cast(rotationQuat);

						glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), part->transform->local_scale);
						glm::mat4 model = translationMatrix * rotationMatrix * scaleMatrix;

						renderer->RenderMesh(part_mr->mesh.get(), part_mr->material.get(), model, cam.GetCamera()->GetViewMat(), cam.GetCamera()->GetProjMat());
					}
				}
			}
		}

		// Unbind
		Luxia::Screen::BindFBO(0);
		Luxia::Screen::BindRBO(0);
		cam_tex->Unbind();
	}

}