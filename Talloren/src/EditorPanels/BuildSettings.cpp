#include "BuildSettings.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

#include "AssetViewPanels/AssetFolderTree.h"
#include "AssetViewPanels/AssetViewer.h"



namespace Talloren::Panels {
	// CORE
	void BuildSettings::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Build Settings Panel: Init");
	}

	void BuildSettings::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Build Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Build Settings";
		TabPopupContextMenu();

		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Build Settings not implemented yet");

		// Yo how the fuck is this gonna work
		if (ImGui::Button("Add Open Scene")) {
			
		}

		if (ImGui::Button("Build")) {
			// Build Project to chosen directory
			// The built project should be the exact same as the editor but with a given project path / file
		}

		ImGui::End();
	}


	void BuildSettings::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void BuildSettings::OnEvent(Luxia::Event& e) {

	}
}
