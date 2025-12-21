#pragma once

#include "IEditorPanel.h"

namespace Talloren::Panel {
	class AssetView : public Talloren::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;
	private:
		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> DrawFolderHierarchy(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene);
		void RefreshAPFs(Talloren::Layers::EditorLayer* editorLayer); // Asset Parent Folders
		std::unordered_map<Luxia::GUID, std::filesystem::path> asset_parent_folders;
		std::filesystem::path selected_folder;
	};
}

