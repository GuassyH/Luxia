#pragma once

#include "Luxia.h"

namespace Talloren::Layers {
	class EditorLayer;
}
namespace Talloren::Panels {
	class AssetView;

	class AssetFolderTree {
	public:
		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> DrawFolderHierarchy(Talloren::Layers::EditorLayer* editorLayer, AssetView* asset_view);
	};
}