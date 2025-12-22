#pragma once

#include "Luxia.h"

namespace Talloren::Layers {
	class EditorLayer;
}
namespace Talloren::Panels {
	class AssetView;

	class AssetViewer {
	public:
		void Init();
		void DrawAssetFiles(Talloren::Layers::EditorLayer* editorLayer, AssetView* asset_view, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw);
		void DrawFileIcon(const std::shared_ptr<Luxia::Assets::Asset> asset, const float cellSize, AssetView* asset_view);

		std::shared_ptr<Luxia::ITexture> mesh_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> mat_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> shader_default_thumbnail;
		std::shared_ptr<Luxia::ITexture> scene_default_thumbnail;
	};
}