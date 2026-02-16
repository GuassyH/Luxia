#include "EditorLayer.h"

// EditorPanels
#include "EditorPanels/GameViewport.h"
#include "EditorPanels/HierarchyPanel.h"
#include "EditorPanels/InspectorPanel.h"
#include "EditorPanels/SceneViewport.h"
#include "EditorPanels/AssetView.h"
#include "EditorPanels/Profiler.h"
#include "EditorPanels/BuildSettings.h"
#include "EditorPanels/ShoutoutPanel.h"

#include "glfw/glfw3.h"

namespace Editor::Layers {
	static std::vector<std::string> layers_to_remove;

	void EditorLayer::ClearSelected() {
		selected_assets.clear();
		UpdateSelectedConditions();
	}
	void EditorLayer::InsertSelected(Luxia::GUID guid){
		selected_assets.insert(guid);
		UpdateSelectedConditions();
	}
	void EditorLayer::EraseSelected(Luxia::GUID guid){
		selected_assets.erase(guid);
		UpdateSelectedConditions();
	}

	void EditorLayer::UpdateSelectedConditions() {
		areNoneSelected = (selected_assets.empty());
		isOneSelected = (selected_assets.size() == 1);
		areMultipleSelected = (selected_assets.size() > 1);
	}

	void EditorLayer::CreateThumbnails() {
		int prevFBO = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		GLboolean depth = glIsEnabled(GL_DEPTH_TEST);

		float clearCol[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, clearCol);

		// Clear existing Thumbnails
		for (auto& [guid, tex] : asset_thumbnails) {
			if (tex) {
				tex->Unload();
			}
		}

		// Create all new ones
		for (auto& [guid, asset] : asset_manager->GetAssetPool()) {
			if (asset) {
				std::shared_ptr<Luxia::ITexture> tex = thumbnailManager.TakeThumbnail(asset, renderer.lock().get());
				if (tex)
					if(tex->IsValid())
						asset_thumbnails[guid] = tex;
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
		if (!depth) glDisable(GL_DEPTH_TEST);
		glClearColor(clearCol[0], clearCol[1], clearCol[2], clearCol[3]);
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}
	
	void EditorLayer::OnAttach() {

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			return;
		}
		thumbnailManager.Init(256, 256);

		LX_CORE_WARN("EditorLayer Attached");
		ImGui::SetCurrentContext(renderer->GetUIRenderer()->GetContext());

		// Just add them all, should use an editor config file but, eh
		PushPanel(std::make_shared<Editor::Panels::GameViewport>());
		PushPanel(std::make_shared<Editor::Panels::HierarchyPanel>());
		PushPanel(std::make_shared<Editor::Panels::InspectorPanel>());
		PushPanel(std::make_shared<Editor::Panels::SceneViewport>());
		PushPanel(std::make_shared<Editor::Panels::AssetView>());

		PlayTex = Luxia::Platform::Assets::CreateTexture();
		PlayTex->LoadFromFile("C:/dev/Luxia/Editor/resources/PlayButton.png");

		PauseTex = Luxia::Platform::Assets::CreateTexture();
		PauseTex->LoadFromFile("C:/dev/Luxia/Editor/resources/PauseButton.png");
	
		RunningTex = Luxia::Platform::Assets::CreateTexture();
		RunningTex->LoadFromFile("C:/dev/Luxia/Editor/resources/Running.png");

		NoImageTex = Luxia::Platform::Assets::CreateTexture();
		NoImageTex->LoadFromFile("C:/dev/Luxia/Editor/resources/asset_icons/NoImage.png");

		if (glBindFramebuffer)
			CreateThumbnails();
	}
	void EditorLayer::OnDetach() {
		LX_CORE_WARN("EditorLayer Detached");
		for (auto panel : panels) {
			panel->Unload(this, scene_manager->GetActiveScene());
		}

		PlayTex->Unload();
		PauseTex->Unload();
		NoImageTex->Unload();
		RunningTex->Unload();
	}
	void EditorLayer::OnUpdate() {
		UpdateSelectedConditions();
	}
	void EditorLayer::OnRender() {
		std::shared_ptr<Luxia::Rendering::IUIRenderer> uiRenderer = renderer->GetUIRenderer();

		static bool opt_fullscreen = true;
		static bool dockspaceOpen = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			// Temp size adjustment to account for top bar
			ImVec2 workPos = viewport->WorkPos;
			ImVec2 workSize = viewport->WorkSize;
			ImGui::SetNextWindowPos(workPos);
			ImGui::SetNextWindowSize(workSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus;

		}

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		if(ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("New Scene")) {
					asset_manager->CreateAssetFile<Luxia::AssetType::Scene>(asset_manager->GetAssetDir(), false, "New Scene");
					PUSH_EVENT(Luxia::MessageSentEvent, "Asset Created");
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					scene_manager->SaveActiveScene();
				}
				
				ImGui::Separator();

				if (ImGui::MenuItem("New Project")) {
					LX_WARN("New Project Menu Item Clicked - Functionality not implemented yet");
				}
				if (ImGui::MenuItem("Open Project")) {
					LX_WARN("Open Project Menu Item Clicked - Functionality not implemented yet");
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) {
					PUSH_EVENT(Luxia::WindowCloseEvent);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Layer")) {
				if (ImGui::BeginMenu("Add Layer")) {

					if (ImGui::MenuItem("Asset View", nullptr, nullptr, !HasPanel<Panels::AssetView>())) {
						PushPanel(std::make_shared<Editor::Panels::AssetView>());
					}
					if (ImGui::MenuItem("Build Settings", nullptr, nullptr, !HasPanel<Panels::BuildSettings>())) {
						PushPanel(std::make_shared<Editor::Panels::BuildSettings>());
					}
					if (ImGui::MenuItem("Game Viewport", nullptr, nullptr, !HasPanel<Panels::GameViewport>())) {
						PushPanel(std::make_shared<Editor::Panels::GameViewport>());
					}
					if (ImGui::MenuItem("Hierarchy", nullptr, nullptr, !HasPanel<Panels::HierarchyPanel>())) {
						PushPanel(std::make_shared<Editor::Panels::HierarchyPanel>());
					}
					if (ImGui::MenuItem("Inspector", nullptr, nullptr, !HasPanel<Panels::InspectorPanel>())) {
						PushPanel(std::make_shared<Editor::Panels::InspectorPanel>());
					}
					if (ImGui::MenuItem("Profiler", nullptr, nullptr, !HasPanel<Panels::Profiler>())) {
						PushPanel(std::make_shared<Editor::Panels::Profiler>());
					}
					if (ImGui::MenuItem("Scene Viewport", nullptr, nullptr, !HasPanel<Panels::SceneViewport>())) {
						PushPanel(std::make_shared<Editor::Panels::SceneViewport>());
					}
					if (ImGui::MenuItem("Shoutouts", nullptr, nullptr, !HasPanel<Panels::ShoutoutPanel>())) {
						PushPanel(std::make_shared<Editor::Panels::ShoutoutPanel>());
					}

					ImGui::EndMenu();
				}
				
				ImGui::EndMenu();
			}

			// Put these buttons in the middle
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float size = ImGui::GetFrameHeight() - 5;
			if (scene_manager->running) {
				ImGui::SetCursorPosX((avail.x / 2.0f) - (size * 2 / 2) - (size + 13));
				ImGui::Image((ImTextureRef)RunningTex->texID, ImVec2(size + 5, size + 5), ImVec2(0, 0), ImVec2(1, 1));
			}
			else
				ImGui::SetCursorPosX((avail.x / 2.0f) - ((size * 2) / 2));



			if (ImGui::ImageButton("##PlayButton", (ImTextureRef)PlayTex->texID, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1))) {
				// If not running, save the scene state and start
				if (!scene_manager->running) {
					scene_manager->SaveActiveScene();
					// Should create a seperate instance or something similar
				}
				else {
					// Should end the current scene and reset it
					auto scene = scene_manager->GetActiveScene();
					if (scene) {
						scene->End();
						scene_manager->SetActiveScene(scene, false);
						// DONT UNLOAD IT. Just reset to the saved state
					}
				}

				scene_manager->running = scene_manager->running ? false : true;
			}
			if (ImGui::ImageButton("##PauseButton", (ImTextureRef)PauseTex->texID, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1))) {
				LX_WARN("Pause button does nothing as of current");
			}

			ImGui::EndMenuBar();
			
		}

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);


		// Create the dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("Editor Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		}

		for (auto& guid : queued_for_refresh) {
			if (asset_thumbnails.contains(guid)) {
				thumbnailManager.RefreshThumbnail(asset_manager->GetAssetPool().find(guid)->second, asset_thumbnails.find(guid)->second, renderer.lock().get());
			}
		}

		// RENDER WINDOWS
		for (auto panel : panels) {
			panel->Render(this, scene_manager->GetActiveScene());
			if (!panel->opened) {
				layers_to_remove.push_back(panel->GetName());
			}
		}

		if (Luxia::Input::IsKeyPressed(LX_KEY_LEFT_CONTROL) && Luxia::Input::IsKeyJustPressed(LX_KEY_S)) {
			if (!scene_manager->running) {
				scene_manager->SaveActiveScene();
				std::filesystem::path p = project_manager->GetProjectPath();
				scene_manager->SaveBuildOrder(p);
			}
			else {
				LX_ERROR("EditorLayer: Cant save scene while it is running!");
			}
		}

		for (auto& layer_name : layers_to_remove) {
			RemovePanel(layer_name);
		}

		layers_to_remove.clear();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::ProfilerRequestEvent>([&](Luxia::ProfilerRequestEvent& event) {
			give_profiler_response = true;
			return false; // Check each event type and update input
			});

		dispatcher.Dispatch<Luxia::WindowCloseEvent>([&](Luxia::WindowCloseEvent& event) {
			if(!scene_manager->running)
				scene_manager->SaveActiveScene();
			return false;
			});

		// FORWARD EVENT WINDOWS
		for (auto panel : panels) {
			panel->OnEvent(e);
		}
	}

}