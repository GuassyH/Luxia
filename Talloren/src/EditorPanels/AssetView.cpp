#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

namespace Talloren::Panel {
	void AssetView::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - AssetView Panel: Init");
	}

	void AssetView::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Asset View");

		// TEMPORARY
		for (auto [guid, asset] : editorLayer->GetAssetManager()->GetAssetPool()) {
			std::ostringstream data; 
			data << asset->name << " - GUID: " << asset->guid;
			ImGui::Text(data.str().c_str());
		}
		// Render all the asset stuff. Not dependant on scene
		// For now maybe just create an item with a description of if its a model, its path, and GUID

		ImGui::End();
	}
	void AssetView::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
	}


	void AssetView::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

	}
}


