#pragma once

#include "IEditorPanel.h"
#include "Luxia.h"
#include <memory>

namespace Talloren::Panels {
	class AssetView : public Talloren::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;
		void RefreshAPFs(Talloren::Layers::EditorLayer* editorLayer); // Asset Parent Folders
		
		std::unordered_map<Luxia::GUID, std::filesystem::path> asset_parent_folders;
		std::filesystem::path selected_folder;
	private:
	};
}

