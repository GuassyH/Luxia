#pragma once

#include "Luxia.h"

namespace Talloren::Layers {
	class EditorLayer;
}
namespace Talloren::Panels {
	class AssetView;

	class AssetViewer {
	public:
		void DrawAssetFiles(Talloren::Layers::EditorLayer* editorLayer, AssetView* asset_view, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw);
	};
}