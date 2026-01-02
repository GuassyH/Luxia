#pragma once

#include "Luxia.h"

namespace Editor::Layers {
	class EditorLayer;
}
namespace Editor::Panels {
	class AssetView;

	class AssetFolderTree {
	public:
		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> DrawFolderHierarchy(Editor::Layers::EditorLayer* editorLayer, AssetView* asset_view);
	};
}