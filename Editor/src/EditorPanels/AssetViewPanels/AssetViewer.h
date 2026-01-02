#pragma once

#include "Luxia.h"

namespace Editor::Layers {
	class EditorLayer;
}
namespace Editor::Panels {
	class AssetView;

	class AssetViewer {
	public:
		void Init();
		void DrawAssetFiles(Editor::Layers::EditorLayer* editorLayer, AssetView* asset_view, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw);
		void DrawFileIcon(const std::shared_ptr<Luxia::Assets::Asset> asset, const float cellSize, Editor::Layers::EditorLayer* editor_layer);
		void DrawFolder(const std::filesystem::path path, const float cellSize, AssetView* asset_view);

		std::shared_ptr<Luxia::ITexture> mesh_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> mat_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> shader_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> scene_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> texture_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> folder_default_thumbnail;
	};
}