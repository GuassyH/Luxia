#include "AssetViewer.h"
#include "EditorLayer.h"
#include "EditorPanels/AssetView.h"


// THIS IS ONLY FOR WINDOWS 
#include <Windows.h>
#include <shobjidl.h>
#include <shellapi.h>

namespace Talloren::Panels {

	static void CopyToClipboard(std::string to_copy) {
		const char* cdata = to_copy.c_str();
		const size_t len = to_copy.size() + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), cdata, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

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

	void AssetViewer::DrawFileIcon(const std::shared_ptr<Luxia::Assets::Asset> asset, const float cellSize, AssetView* asset_view) {
		// Thumbnail
		ImGui::PushID(asset->guid);

		ImGui::BeginGroup(); // Start group so selectable spans all content

		unsigned int thumbnail = 0;
		bool hasThumbnail = false;

		// Inefficient but works
		auto texasset = std::dynamic_pointer_cast<Luxia::ITexture>(asset);
		auto meshasset = std::dynamic_pointer_cast<Luxia::Mesh>(asset);
		auto matasset = std::dynamic_pointer_cast<Luxia::IMaterial>(asset);
		auto shaderasset = std::dynamic_pointer_cast<Luxia::IShader>(asset);
		auto sceneasset = std::dynamic_pointer_cast<Luxia::Scene>(asset);

		if (texasset) {
			thumbnail = texasset->texID;
			hasThumbnail = texasset->IsValid();
		}
		else if (meshasset) {
			thumbnail = mesh_default_thumbnail->texID;
			hasThumbnail = mesh_default_thumbnail->IsValid();
		}
		else if (matasset) {
			thumbnail = mat_default_thumbnail->texID;
			hasThumbnail = mat_default_thumbnail->IsValid();
		}
		else if (shaderasset) {
			thumbnail = shader_default_thumbnail->texID;
			hasThumbnail = shader_default_thumbnail->IsValid();
		}
		else if (sceneasset) {
			thumbnail = scene_default_thumbnail->texID;
			hasThumbnail = scene_default_thumbnail->IsValid();
		}

		if (hasThumbnail)
			ImGui::Image((ImTextureRef)thumbnail, ImVec2(cellSize, cellSize)); // Replace nullptr with your thumbnail
		else
			ImGui::Image(nullptr, ImVec2(cellSize, cellSize)); // Replace nullptr with your thumbnail


		// Get size of the group
		ImVec2 groupSize = ImVec2(ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y + 5);

		// Make the selectable span the entire group
		bool is_selected = asset_view->selected_assets.contains(asset->guid);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - groupSize.y);
		if (ImGui::Selectable("##AssetSelectable", is_selected, ImGuiSelectableFlags_AllowItemOverlap, groupSize)) {
			if (ImGui::GetIO().KeyCtrl) {
				if (is_selected)
					asset_view->selected_assets.erase(asset->guid);
				else
					asset_view->selected_assets.insert(asset->guid);
			}
			else {
				asset_view->selected_assets.clear();
				asset_view->selected_assets.insert(asset->guid);
			}
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			if (ImGui::GetIO().KeyCtrl) {
				if (!is_selected)
					asset_view->selected_assets.insert(asset->guid);
			}
			else {
				asset_view->selected_assets.clear();
				asset_view->selected_assets.insert(asset->guid);
			}
		}

		// Draw text below image
		ImGui::TextWrapped(asset->name.c_str());

		ImGui::EndGroup(); // End group

		ImGui::PopID();
	}


	void AssetViewer::Init() {
		mesh_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		mesh_default_thumbnail->LoadFromFile("resources/AssetIcons/MeshDefaultThumbnail.png");

		mat_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		mat_default_thumbnail->LoadFromFile("resources/AssetIcons/MatDefaultThumbnail.png");

		shader_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		shader_default_thumbnail->LoadFromFile("resources/AssetIcons/ShaderDefaultThumbnail.png");

		scene_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		scene_default_thumbnail->LoadFromFile("resources/AssetIcons/SceneDefaultThumbnail.png");
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

		// Should draw sub folders here

		for (auto& [guid, asset] : assets_to_draw) {
			if (!asset) continue;

			DrawFileIcon(asset.lock(), cellSize, asset_view);

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		if (ImGui::IsWindowHovered()) {
			if (ImGui::GetIO().MouseClicked[0]) {
				if (!ImGui::IsAnyItemHovered()) {
					asset_view->selected_assets.clear();
				}
			}		
			if (ImGui::GetIO().MouseClicked[1]) {
				if (!ImGui::IsAnyItemHovered()) {
					asset_view->selected_assets.clear();
				}
			}
		}

		bool areNoneSelected = (asset_view->selected_assets.empty());
		bool isOneSelected = (asset_view->selected_assets.size() == 1);
		bool areMultipleSelected = (asset_view->selected_assets.size() > 1);

		// Popup
		if (ImGui::BeginPopupContextWindow("Asset Viewer", ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Copy", nullptr, nullptr, isOneSelected)) {
				for (auto& guid : asset_view->selected_assets) {
					CopyToClipboard(std::to_string((uint64_t)guid));
				}
			}
			if (ImGui::MenuItem("Open in File-Explorer", nullptr, nullptr, isOneSelected)) {
				for (auto& guid : asset_view->selected_assets) {
					auto it = asset_view->asset_parent_folders.find(guid);
					if (it != asset_view->asset_parent_folders.end()) {
						const std::string pathStr = it->second.string(); // keep the string alive
						ShellExecuteA(nullptr, "open", pathStr.c_str(), nullptr, nullptr, SW_SHOW);
					}
				}
			}

			if (ImGui::MenuItem("Import", nullptr, nullptr, areNoneSelected)) {
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

			if (ImGui::BeginMenu("Create", areNoneSelected)) {
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