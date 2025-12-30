#include "AssetFolderTree.h"
#include "EditorLayer.h"
#include "EditorPanels/AssetView.h"

namespace Talloren::Panels {
	// Should cache later, not that important now with small trees
	// Same Setup as hierarchy inspector
	static void DrawPathSelectable(const std::filesystem::directory_entry& entry, std::filesystem::path& sel_fol) {
		if (!entry.is_directory()) return;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		if (sel_fol == entry.path()) flags |= ImGuiTreeNodeFlags_Selected;

		std::ostringstream label; label << "../" << entry.path().stem().string().c_str();
		ImGui::PushID(entry.path().string().c_str());
		bool open = ImGui::TreeNodeEx(label.str().c_str(), flags);
		ImGui::PopID();

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			sel_fol = entry.path();
		}

		if (open) {
			std::vector<std::filesystem::directory_entry> entries;
			for (const auto& e : std::filesystem::directory_iterator(entry.path()))
				entries.push_back(e);

			// Sort alphabetically
			std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.path().filename() < b.path().filename(); });

			std::error_code ec;
			for (const auto& subentry : std::filesystem::directory_iterator(entry.path(), ec)) {
				if (ec) continue;
				DrawPathSelectable(subentry, sel_fol);
			}
			ImGui::TreePop();
		}
	}

	std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> AssetFolderTree::DrawFolderHierarchy(Talloren::Layers::EditorLayer* editorLayer, AssetView* asset_view) {
		// Popup
		if (ImGui::BeginPopupContextWindow("Asset Hierarchy Viewer", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Refresh All")) {
				asset_view->RefreshAPFs(editorLayer);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Loop through each sub-directory in the projects asset directory
		std::error_code ec;
		for (auto& entry : std::filesystem::directory_iterator(editorLayer->GetProjectManager()->GetProjectPath(), ec)) {
			if (ec) continue;
			DrawPathSelectable(entry, asset_view->selected_folder);
		}

		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> assets_to_draw;
		if (!asset_view->selected_folder.empty() && std::filesystem::exists(asset_view->selected_folder)) {
			// If its a valid folder, go through each asset, and see if its guid in the a_p_f path is the selected folder
			for (auto& [guid, asset] : editorLayer->GetAssetManager()->GetAssetPool()) {
				if (asset) {
					// if its the same condition itll crash
					auto it = asset_view->asset_parent_folders.find(guid);
					if (it == asset_view->asset_parent_folders.end())
						continue;

					if (it->second == asset_view->selected_folder) {
						assets_to_draw[guid] = asset;
					}
				}
			}
		}

		return assets_to_draw;
	}
}