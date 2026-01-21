#include "BuildSettings.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

#include "AssetViewPanels/AssetFolderTree.h"
#include "AssetViewPanels/AssetViewer.h"

namespace Editor::Panels {
	static int selected_scene_index;

	// CORE
	void BuildSettings::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Build Settings Panel: Init");
		selected_scene_index = 0;
	}

	void BuildSettings::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Build Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Build Settings";
		TabPopupContextMenu();

		auto sm = editorLayer->GetSceneManager();

		ImGui::BeginChild(ImGuiID("##BuildOrderChild"), ImVec2(ImGui::GetContentRegionAvail().x, 50));
		ImGui::BeginChildFrame(ImGuiID("##BuildOrderChildFrame"), ImVec2(ImGui::GetContentRegionAvail().x, 50));
		int index = 0;
		for (auto& guid : sm->build_order) {
			std::ostringstream ui; ui << index << ": " << sm->scene_files.find(guid)->second->assets[0]->name;
			if (ImGui::Selectable(ui.str().c_str(), index == selected_scene_index)) {
				selected_scene_index = index;
			}
			index++;
		}
		if (ImGui::BeginPopupContextWindow("BuildIndexPopup")) {
			if (ImGui::Button("Remove")) {
				sm->RemoveFromBuildOrder(selected_scene_index);
			}

			ImGui::EndPopup();
		}

		ImGui::EndChildFrame();
		ImGui::EndChild();

		// Yo how the fuck is this gonna work
		if (ImGui::Button("Add Open Scene")) {
			if (scene) {
				sm->AddToBuildOrder(scene->scene_file->guid);
			}
		}

		if (ImGui::Button("Build")) {
			// Build Project to chosen directory
			// The built project should be the exact same as the editor but with a given project path / file
		}

		ImGui::End();
	}


	void BuildSettings::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void BuildSettings::OnEvent(Luxia::Event& e) {

	}
}
