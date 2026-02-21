#include "SceneViewport.h"
#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"
#include "EditorScripts/SceneShooterTest.h"

namespace Editor::Panels {

	static std::shared_ptr<Luxia::ITexture> fbo_pick_tex = Luxia::Platform::Assets::CreateTexture();
	static std::shared_ptr<Luxia::ITexture> selection_fbo = Luxia::Platform::Assets::CreateTexture();
	static std::shared_ptr<Luxia::IMaterial> outline_mat = Luxia::Platform::Assets::CreateMaterial();
	static std::shared_ptr<Luxia::IShader> outline_shader = nullptr;
	static Luxia::Physics::RayCastHit last_ray_hit = Luxia::Physics::RayCastHit();


	enum EditTool {
		Translate = 0,
		Rotate = 1,
		Scale = 2
	};
	static EditTool editTool = EditTool::Translate;
	static std::vector<std::unique_ptr<Gizmos::GizmoCollection>> edit_tools;

	static void SetTool(int index, Editor::Layers::EditorLayer* editorLayer) {
		for (int i = 0; i < 3; i++) {
			auto collection_ptr = edit_tools[i].get();
			for (auto& gizmo_behaviour : collection_ptr->behaviours) {
				gizmo_behaviour->gizmo_part->is_active = (i != index ? false : true);
			}
		}
	}


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
			if (scene->GetReg().try_get<Luxia::Components::Transform>(entt::entity(pickedID))) {
				result = scene->GetFromEntity<Luxia::Components::Transform>(entt::entity(pickedID)).ent_guid;
			}
			else {
				LX_CORE_ERROR("Picked entity with ID {} but it doesn't have a transform component", pickedID);
			}
		}

		Luxia::Screen::BindFBO(0);
		Luxia::Screen::BindRBO(0);
		output_texture->Unbind();

		return result;
	}

	/// CORE
	void SceneViewport::RenderTopBar(Editor::Layers::EditorLayer* editorLayer) {
		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("Camera")) {
				ImGui::Checkbox("Use Skybox", &cam_ent->GetComponent<Luxia::Components::Camera>().useSkybox);
				if(!cam_ent->GetComponent<Luxia::Components::Camera>().useSkybox)
					ImGui::ColorEdit4("Clear Color", &cam_ent->GetComponent<Luxia::Components::Camera>().clearColor.r);
				ImGui::SliderFloat("FOV", &cam_ent->GetComponent<Luxia::Components::Camera>().FOVdeg, 1.0f, 179.0f);
				ImGui::DragFloat("Near Plane", &cam_ent->GetComponent<Luxia::Components::Camera>().nearPlane, 0.01f, 0.01f, cam_ent->GetComponent<Luxia::Components::Camera>().farPlane - 0.1f);
				ImGui::DragFloat("Far Plane", &cam_ent->GetComponent<Luxia::Components::Camera>().farPlane, 1.0f, cam_ent->GetComponent<Luxia::Components::Camera>().nearPlane + 0.1f, 10000.0f);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}
	
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
		cam_ent->AddComponent<Editor::Scripts::SceneShooterTest>();

		fbo_pick_tex->CreateFBOTex(1920, 1080);
		selection_fbo->CreateDepthTex(1920, 1080);

		outline_shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/Editor/resources/gizmos/shaders/outline.frag", "C:/dev/Luxia/Editor/resources/gizmos/shaders/outline.vert");

		outline_mat->shader = outline_shader;
		outline_mat->guid = Luxia::GUID();
		outline_mat->color = glm::vec4(1.0f, 0.55f, 0.2f, 1.0f);


		InitGizmos(editorLayer);
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

		// Unload all gizmos (and other physics stuff) from physics world
		auto& body_interface = editorLayer->physicsWorld->jphSystem.GetBodyInterface();
		auto view = editorLayer->editor_reg.view<Luxia::Components::RigidBody>();
		for (auto entity : view) {
			auto& rb = editorLayer->editor_reg.get<Luxia::Components::RigidBody>(entity);
			rb.UnloadBody(body_interface);
		}
	}

	void SceneViewport::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar);
		name = "Scene View";
		TabPopupContextMenu();

		Luxia::Components::Camera& cam = cam_ent->GetComponent<Luxia::Components::Camera>();
		Editor::Scripts::SceneCameraScript& cam_script = cam_ent->GetComponent<Editor::Scripts::SceneCameraScript>();
		cam.width = static_cast<int>(ImGui::GetWindowWidth());
		cam.height = static_cast<int>(ImGui::GetWindowHeight() - 20);  // Some sort of padding
		cam.transform->UpdateMatrix();
		glm::vec2 imageSize = glm::vec2(cam.width, cam.height);

		// Set the image
		if (output_texture) { 
			RenderTopBar(editorLayer);
			ImVec2 windowSize = ImGui::GetContentRegionAvail();

			ImVec2 imagePosInWindow;
			imagePosInWindow.x = ((windowSize.x - imageSize.x) * 0.5f) + ImGui::GetCursorPosX();
			imagePosInWindow.y = ((windowSize.y - imageSize.y) * 0.5f) + ImGui::GetCursorPosY();

			ImGui::SetCursorPosX(imagePosInWindow.x);
			ImGui::SetCursorPosY(imagePosInWindow.y);

			ImGui::Image((ImTextureID)(intptr_t)output_texture->texID, ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0));


			if (ImGui::IsWindowHovered() && ImGui::IsItemHovered()) {

				// Picking
				if (scene) {
					glm::vec2 window_pos = glm::vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 20);
					glm::vec2 rp = Luxia::Screen::GetMousePosRect(window_pos, imageSize, imageSize, Luxia::Input::GetMousePosition());
					glm::vec3 ray_dir = Luxia::Screen::GetRayDir(Luxia::Screen::GetMousePosRectClamped(window_pos, glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), Luxia::Input::GetMousePosition()), &cam);
					
					if (!GizmoRaycasts(editorLayer, scene.get(), ray_dir)) {
						if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
							// FBO Picking
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

					if (Luxia::Input::IsKeyJustPressed(LX_KEY_H) && scene) {
						if(editorLayer->GetSceneManager()->running)
							cam_ent->transform->GetComponent<Editor::Scripts::SceneShooterTest>().Shoot(*scene, editorLayer->GetAssetManager().get());
					}

					cam_script.Look();
					cam_script.Move();
				}

				if (Luxia::Input::IsKeyJustPressed(LX_KEY_F)) {
					if (focused_t) {
						cam.transform->local_position = focused_t->local_position + (cam.transform->forward * -10.0f);
					}
				}
			}
		}

		ImGui::End();

		if (scene) {
			// Render
			std::shared_ptr<Luxia::ITexture> tex = cam.Render(scene, editorLayer->GetRenderer());
			RenderGizmos(editorLayer, scene.get(), tex);
			// Push for next frame
			PUSH_EVENT(Luxia::RenderCameraEvent, tex, false, true);
		}
		
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
		Gizmos::GizmoResources::Init("C:/dev/Luxia/Editor/resources/gizmos");

		// Gizmos::TranslateCollection(&editorLayer->editor_reg);
		edit_tools.push_back(std::make_unique<Gizmos::TranslateCollection>(&editorLayer->editor_reg));
		edit_tools.push_back(std::make_unique<Gizmos::RotateCollection>(&editorLayer->editor_reg));
		edit_tools.push_back(std::make_unique<Gizmos::ScaleCollection>(&editorLayer->editor_reg));

		// Add all gizmos (and other physics stuff) to physics world for raycasting
		auto& body_interface = editorLayer->physicsWorld->jphSystem.GetBodyInterface();
		auto view = editorLayer->editor_reg.view<Luxia::Components::RigidBody>();
		for (auto entity : view) {
			auto& rb = editorLayer->editor_reg.get<Luxia::Components::RigidBody>(entity);
			rb.InitBody(body_interface);
		}

		SetTool(0, editorLayer);
	}

	void SceneViewport::RenderGizmos(Editor::Layers::EditorLayer* editorLayer, Luxia::Scene* scene, std::shared_ptr<Luxia::ITexture> cam_tex) {
		// Get the important stuff
		Luxia::Rendering::IRenderer* renderer = editorLayer->GetRenderer().get();
		auto& cam = cam_ent->GetComponent<Luxia::Components::Camera>();
		cam.transform->UpdateMatrix();

		auto& body_interface = editorLayer->physicsWorld->jphSystem.GetBodyInterface();

		// Rebind
		Luxia::Screen::BindFBO(cam_tex->GetFBO());
		Luxia::Screen::BindRBO(cam_tex->GetRBO());

		if (cam.width != selection_fbo->GetWidth() || cam.height != selection_fbo->GetHeight()) {
			selection_fbo->Unload();
			selection_fbo->CreateDepthTex(cam.width, cam.height);
		}


		/// With Depth
	
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
		
		// Selecting Gizmo
		if (!Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_2) && !Luxia::Input::IsKeyPressed(LX_KEY_LEFT_CONTROL)){
			if (Luxia::Input::IsKeyJustPressed(LX_KEY_W)) {
				editTool = EditTool::Translate;
				SetTool(editTool, editorLayer);
			}
			else if (Luxia::Input::IsKeyJustPressed(LX_KEY_S)) {
				editTool = EditTool::Scale;
				SetTool(editTool, editorLayer);
			}
			else if (Luxia::Input::IsKeyJustPressed(LX_KEY_R)) {
				editTool = EditTool::Rotate;
				SetTool(editTool, editorLayer);
			}
		}


		// Update Gizmos
		auto gb_view = editorLayer->editor_reg.view<Gizmos::GizmoBehaviour>();
		for (auto entity : gb_view) {
			auto& gb = editorLayer->editor_reg.get<Gizmos::GizmoBehaviour>(entity);
			if (gb.gizmo_part && gb.transform->enabled) {
				if (gb.gizmo_part->ShouldUpdate(editorLayer, &cam, scene)) {
					gb.gizmo_part->OnUpdate(editorLayer, &cam, scene);
				}
			}
		}

		// If there was a hit last frame
		if (scene) {
			if (active_gizmo_part) {
				if (Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_1)) {
					active_gizmo_part->OnDrag(last_ray_hit, editorLayer, this);
				}
			}
			else if (last_ray_hit.hit) {
				for (auto entity : gb_view) {
					if (auto rb = editorLayer->editor_reg.try_get<Luxia::Components::RigidBody>(entity)) {
						if (rb->body->GetID() == last_ray_hit.bodyID) {
							if (auto gb = rb->transform->TryGetComponent<Gizmos::GizmoBehaviour>()) {
								gb->gizmo_part->OnHover();
								if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
									gb->gizmo_part->OnClick(last_ray_hit, editorLayer, this);
								}
								if (Luxia::Input::IsMouseButtonPressed(LX_MOUSE_BUTTON_1)) {
									gb->gizmo_part->OnDrag(last_ray_hit, editorLayer, this);
								}
							}
						}
					}
				}
			}
		}

		// Render Gizmos
		for (auto entity : gb_view) {
			auto& gb = editorLayer->editor_reg.get<Gizmos::GizmoBehaviour>(entity);
			if (gb.gizmo_part && gb.transform->enabled) {
				if (gb.gizmo_part->ShouldRender(editorLayer, &cam, scene)) {
					gb.gizmo_part->OnRender(editorLayer->GetRenderer().get(), editorLayer, &cam);
					gb.gizmo_part->OnUnhover(); // If hovered check?
				}
			}
		}

		// Check unclick
		if (Luxia::Input::IsMouseButtonJustReleased(LX_MOUSE_BUTTON_1)) {
			for (auto entity : gb_view) {
				auto& gb = editorLayer->editor_reg.get<Gizmos::GizmoBehaviour>(entity);
				if (gb.gizmo_part && gb.transform->enabled) {
					gb.gizmo_part->OnUnclick(last_ray_hit, editorLayer, this);
				}
			}
		}

		// Unbind
		Luxia::Screen::BindFBO(0);
		Luxia::Screen::BindRBO(0);
		cam_tex->Unbind();
	}



	bool SceneViewport::GizmoRaycasts(Editor::Layers::EditorLayer* editorLayer, Luxia::Scene* scene, glm::vec3 ray_dir) {
		editorLayer->physicsWorld->RayCast(cam_ent->transform->world_position, ray_dir, cam_ent->GetComponent<Luxia::Components::Camera>().farPlane, &last_ray_hit);
		if (last_ray_hit.hit) {
			return true;
		}
		return false;
	}
}