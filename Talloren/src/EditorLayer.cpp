#include "EditorLayer.h"
#include "EditorPanels/GameViewport.h"
#include "EditorPanels/HierarchyPanel.h"
#include "EditorPanels/InspectorPanel.h"
#include "EditorPanels/SceneViewport.h"
#include "EditorPanels/AssetView.h"


namespace Talloren::Layers {
	
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
	
	void EditorLayer::OnAttach() {
		LX_CORE_WARN("EditorLayer Attached");
		ImGui::SetCurrentContext(renderer->GetUIRenderer()->GetContext());

		PushPanel(std::make_shared<Talloren::Panels::GameViewport>());
		PushPanel(std::make_shared<Talloren::Panels::HierarchyPanel>());
		PushPanel(std::make_shared<Talloren::Panels::InspectorPanel>());
		PushPanel(std::make_shared<Talloren::Panels::SceneViewport>());
		PushPanel(std::make_shared<Talloren::Panels::AssetView>());
	}
	void EditorLayer::OnDetach() {
		LX_CORE_WARN("EditorLayer Detached");
		for (auto panel : panels) {
			panel->Unload(this, scene_manager->GetActiveScene());
		}
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
					asset_manager->CreateAssetFile<Luxia::AssetType::SceneType>(asset_manager->GetAssetDir(), false, "New Scene");
					PUSH_EVENT(Luxia::MessageSentEvent, "Asset Created");
				}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
					scene_manager->SaveActiveScene();
				}
				
				ImGui::Separator();

				if (ImGui::MenuItem("New Project")) {
					// project_manager->CloseProject();
					// project_manager->NewProject("E:/BuiltLuxia/NewProject", "New_Project_Name");
					LX_WARN("New Project Menu Item Clicked - Functionality not implemented yet");
				}
				if (ImGui::MenuItem("Open Project")) {
					// project_manager->CloseProject();
					// project_manager->OpenProject("E:/BuiltLuxia/NewProject");
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


			if (ImGui::BeginMenu("Build")) {
				// Should be able to build project here
				// And Also switch Build Configurations
				// And importantly order by index scenes
				ImGui::EndMenu();
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

		
		// RENDER WINDOWS
		for (auto panel : panels) {
			panel->Render(this, scene_manager->GetActiveScene());
		}

		if (Luxia::Input::IsKeyPressed(LX_KEY_LEFT_CONTROL) && Luxia::Input::IsKeyJustPressed(LX_KEY_S)) {
			scene_manager->SaveActiveScene();
		}

		ImGui::End();


	}

	void EditorLayer::OnEvent(Luxia::Event& e) {
		// FORWARD EVENT WINDOWS
		for (auto panel : panels) {
			panel->OnEvent(e);
		}
	}

}