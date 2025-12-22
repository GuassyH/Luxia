#include "AssetViewer.h"
#include "EditorLayer.h"
#include "EditorPanels/AssetView.h"

// THIS IS ONLY FOR WINDOWS 
#include <Windows.h>
#include <shobjidl.h>

namespace Talloren::Panels {


	static std::string OpenFileDialogue() {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(hr)) return {};

		IFileOpenDialog* pFileOpen = nullptr;
		hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));

		std::wstring filePath;

		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem = nullptr;
				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath = nullptr;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						filePath = pszFilePath;
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}

		CoUninitialize();

		if (filePath.empty()) return {};

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), (int)filePath.size(), nullptr, 0, nullptr, nullptr);
		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), (int)filePath.size(), result.data(), sizeNeeded, nullptr, nullptr);

		return result;
	}

	void AssetViewer::DrawAssetFiles(Talloren::Layers::EditorLayer* editorLayer, AssetView* asset_view, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw)
	{
		// For each asset in the assets_to_draw map, draw it (will be more polished later)

		float cellSize = 100.0f;
		float padding = 10.0f;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelWidth / (cellSize + padding));
		if (columns < 1) columns = 1;

		ImGui::Columns(columns, nullptr, false);

		for (auto& [guid, asset] : assets_to_draw) {
			if (!asset) continue;

			// Thumbnail
			ImGui::Image(nullptr, ImVec2(cellSize, cellSize));

			// Name under icon
			ImGui::TextWrapped(asset->name.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		// Popup
		if (ImGui::BeginPopupContextWindow("Asset Viewer", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Import")) {
				// Open Folder, if you choose something of supported type, import correctly
				std::string fp = OpenFileDialogue();
				std::filesystem::path filepath = fp;
				if (!filepath.empty() && std::filesystem::exists(filepath)) {
					LX_INFO("Selected Path: {}", filepath.string());
					editorLayer->GetAssetManager()->Import(filepath, false, filepath.filename().string());
				}
				asset_view->RefreshAPFs(editorLayer);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginMenu("Create")) {
				// Should be created in the folder you are in
				if (ImGui::MenuItem("Material")) {
					if (!asset_view->selected_folder.empty() && std::filesystem::exists(asset_view->selected_folder)) {
						editorLayer->GetAssetManager()->CreateAssetFile<Luxia::AssetType::MaterialType>(asset_view->selected_folder, false, "NewMaterial");
						asset_view->RefreshAPFs(editorLayer);
					}
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Scene")) {
					// its very weird, need to look into scene management
					// asset_manager->CreateAssetFile<Luxia::AssetType::SceneType>("scenes", true, "NewScene");
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}

}