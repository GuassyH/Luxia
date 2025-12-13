#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Talloren::Panel {
	void AssetView::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - AssetView Panel: Init");
	}

	void AssetView::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Asset View");

		// Render all the asset stuff. Not dependant on scene

		ImGui::End();
	}
	void AssetView::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
	}


	void AssetView::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

	}
}