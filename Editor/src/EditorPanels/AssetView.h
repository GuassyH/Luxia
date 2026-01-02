#pragma once

#include "IEditorPanel.h"
#include "Luxia.h"
#include <memory>

namespace Editor::Panels {
	class AssetView : public Editor::IEditorPanel {
	public:
		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;
		void RefreshAPFs(Editor::Layers::EditorLayer* editorLayer, bool reset_dir = false); // Asset Parent Folders
		
		std::unordered_map<Luxia::GUID, std::filesystem::path> asset_parent_folders;
		std::filesystem::path selected_folder;
	private:
	};
}

