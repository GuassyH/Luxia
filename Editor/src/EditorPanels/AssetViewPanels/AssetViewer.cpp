#include "AssetViewer.h"
#include "EditorLayer.h"
#include "EditorPanels/AssetView.h"


// THIS IS ONLY FOR WINDOWS 
#include <Windows.h>
#include <shobjidl.h>
#include <shellapi.h>

namespace Editor::Panels {

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

	static void OpenAsset(Luxia::GUID guid, Editor::Layers::EditorLayer* editorLayer) {
		// Validate asset manager and scene manager pointers
		auto assetManager = editorLayer->GetAssetManager();
		if (!assetManager) {
			LX_ERROR("OpenAsset: AssetManager is null");
			return;
		}

		const auto& assetPool = assetManager->GetAssetPool();
		auto it = assetPool.find(guid);
		if (it == assetPool.end() || !it->second) {
			LX_ERROR("OpenAsset: asset not found for GUID {}", (uint64_t)guid);
			return;
		}

		auto asset = it->second;

		switch (asset->type) {
		case Luxia::AssetType::SceneType: {
			auto sceneManager = editorLayer->GetSceneManager();
			if (!sceneManager) {
				LX_ERROR("OpenAsset: SceneManager is null");
				return;
			}

			bool found = false;
			for (auto& sf : sceneManager->scene_files) {
				if (!sf) continue;
				if (sf->assets.empty()) continue;
				// defensive check
				if (sf->assets[0]) {
					if (sf->assets[0]->guid == guid) {
						auto loaded = sceneManager->SetActiveScene(sf);
						if (!loaded) {
							LX_WARN("OpenAsset: SceneManager::SetActiveScene failed for asset GUID {}", (uint64_t)guid);
						}
						found = true;
						break;
					}
				}
			}
			if (!found) {
				LX_WARN("OpenAsset: Scene asset GUID {} not present in SceneManager::scene_files", (uint64_t)guid);
			}
			break;
		}
		default:
			LX_ERROR("Opening asset type not implemented yet!");
			break;
		}
	}

	void AssetViewer::DrawFolder(const std::filesystem::path path, const float cellSize, AssetView* asset_view) {
		std::string id = path.string() + "FolderIcon";
		ImGui::PushID(id.c_str());

		ImGui::BeginGroup(); // Start group so selectable spans all content

		ImGui::Image((ImTextureRef)folder_default_thumbnail->texID, ImVec2(cellSize, cellSize)); // Replace nullptr with your thumbnail

		// Get size of the group
		ImVec2 groupSize = ImVec2(ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y + 5);

		// Make the selectable span the entire group
		bool selected = asset_view->selected_folder == path;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - groupSize.y);
		if (ImGui::Selectable("##FolderSelectable", selected, ImGuiSelectableFlags_AllowItemOverlap, groupSize)) {
			asset_view->selected_folder = path;
		}

		// Draw text below image
		std::string name_text = path.filename().replace_extension("").string().c_str();
		ImGui::TextWrapped(name_text.c_str());

		ImGui::EndGroup(); // End group

		ImGui::PopID();
	}

	void AssetViewer::DrawFileIcon(const std::shared_ptr<Luxia::Assets::Asset> asset, const float cellSize, Editor::Layers::EditorLayer* editor_layer) {
		// Thumbnail
		// Use stable string id for ImGui PushID to avoid UB from implicit conversions
		std::string id = std::to_string((uint64_t)asset->guid);
		ImGui::PushID(id.c_str());

		ImGui::BeginGroup(); // Start group so selectable spans all content

		unsigned int thumbnail = 0;
		bool hasThumbnail = false;

		switch (asset->type) {
		case Luxia::AssetType::TextureType: {
			auto texasset = std::dynamic_pointer_cast<Luxia::ITexture>(asset);
			if (texasset) {
				if (texasset->IsValid()) {
					thumbnail = texasset->texID;
					hasThumbnail = texasset->IsValid();
				}
				else {
					thumbnail = texture_default_thumbnail->texID;
					hasThumbnail = texture_default_thumbnail->IsValid();
				}
			}
			else {
				thumbnail = texture_default_thumbnail->texID;
				hasThumbnail = texture_default_thumbnail->IsValid();
			}
			break;
		}
		case Luxia::AssetType::MeshType: {
			thumbnail = mesh_default_thumbnail->texID;
			hasThumbnail = mesh_default_thumbnail->IsValid();
			break;
		}
		case Luxia::AssetType::MaterialType: {
			thumbnail = mat_default_thumbnail->texID;
			hasThumbnail = mat_default_thumbnail->IsValid();
			break;
		}
		case Luxia::AssetType::ShaderType: {
			thumbnail = shader_default_thumbnail->texID;
			hasThumbnail = shader_default_thumbnail->IsValid();
			break;
		}
		case Luxia::AssetType::SceneType: {
			thumbnail = scene_default_thumbnail->texID;
			hasThumbnail = scene_default_thumbnail->IsValid();
			break;
		}
		default:
			break;
		}


		if (hasThumbnail)
			ImGui::Image((ImTextureRef)thumbnail, ImVec2(cellSize, cellSize)); // Replace nullptr with your thumbnail
		else
			ImGui::Image(nullptr, ImVec2(cellSize, cellSize)); // Replace nullptr with your thumbnail


		// Get size of the group
		ImVec2 groupSize = ImVec2(ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y + 5);

		// Make the selectable span the entire group
		bool is_selected = editor_layer->selected_assets.contains(asset->guid);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - groupSize.y);
		if (ImGui::Selectable("##AssetSelectable", is_selected, ImGuiSelectableFlags_AllowItemOverlap, groupSize)) {
			if (ImGui::GetIO().KeyCtrl) {
				if (is_selected)
					editor_layer->EraseSelected(asset->guid);
				else
					editor_layer->InsertSelected(asset->guid);
			}
			else {
				editor_layer->ClearSelected();
				editor_layer->InsertSelected(asset->guid);
			}
		}
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			if (ImGui::GetIO().KeyCtrl) {
				if (!is_selected)
					editor_layer->InsertSelected(asset->guid);
			}
			else {
				editor_layer->ClearSelected();
				editor_layer->InsertSelected(asset->guid);
			}
		}

		// Draw text below image
		ImGui::TextWrapped(asset->name.c_str());

		ImGui::EndGroup(); // End group

		ImGui::PopID();
	}


	void AssetViewer::Init() {
		mesh_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		mesh_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/MeshDefaultThumbnail.png");

		mat_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		mat_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/MatDefaultThumbnail.png");

		shader_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		shader_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/ShaderDefaultThumbnail.png");

		scene_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		scene_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/SceneDefaultThumbnail.png");

		texture_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		texture_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/TextureDefaultThumbnail.png");

		folder_default_thumbnail = Luxia::Platform::Assets::CreateTexture();
		folder_default_thumbnail->LoadFromFile("C:/dev/Luxia/Editor/resources/AssetIcons/FolderDefaultThumbnail.png");
	}

	void AssetViewer::DrawAssetFiles(Editor::Layers::EditorLayer* editorLayer, AssetView* asset_view, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw)
	{
		// For each asset in the assets_to_draw map, draw it (will be more polished later)
		float cellSize = 100.0f;
		float padding = 10.0f;
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float panelWidth = avail.x;

		if (avail.x <= 1.0f || avail.y <= 1.0f)
			return;

		int columns = (int)(panelWidth / (cellSize + padding));
		if (columns < 1) columns = 1;

		ImGui::Columns(columns, nullptr, false);

		// Should draw sub folders here

		std::error_code ec;
		for (auto& entry : std::filesystem::directory_iterator(asset_view->selected_folder, ec)) {
			if (ec) continue;
			if (!entry.is_directory()) continue;
			DrawFolder(entry.path(), cellSize, asset_view);
			ImGui::NextColumn();
		}

		for (auto& [guid, asset] : assets_to_draw) {
			if (!asset) continue;

			DrawFileIcon(asset.lock(), cellSize, editorLayer);

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		if (ImGui::IsWindowHovered()) {
			if (ImGui::GetIO().MouseClicked[0]) {
				if (!ImGui::IsAnyItemHovered()) {
					editorLayer->ClearSelected();
				}
			}		
			if (ImGui::GetIO().MouseClicked[1]) {
				if (!ImGui::IsAnyItemHovered()) {
					editorLayer->ClearSelected();
				}
			}
		}

		static bool openRenamePopup = false;

		// Asset Context Menu
		if (ImGui::BeginPopupContextWindow("Asset Viewer", ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Copy", nullptr, nullptr, editorLayer->isOneSelected)) {
				Luxia::GUID e_guid = *editorLayer->selected_assets.begin();
				CopyToClipboard(std::to_string((uint64_t)e_guid));
			}
			if (ImGui::MenuItem("Open", nullptr, nullptr, editorLayer->isOneSelected)) {
				Luxia::GUID e_guid = *editorLayer->selected_assets.begin();
				OpenAsset(e_guid, editorLayer);
			}
			if (ImGui::MenuItem("Open in File-Explorer", nullptr, nullptr, editorLayer->isOneSelected)) {
				Luxia::GUID e_guid = *editorLayer->selected_assets.begin();
				auto it = asset_view->asset_parent_folders.find(e_guid);
				if (it != asset_view->asset_parent_folders.end()) {
					const std::string pathStr = it->second.string(); // keep the string alive
					ShellExecuteA(nullptr, "open", pathStr.c_str(), nullptr, nullptr, SW_SHOW);
				}
			}
			if (ImGui::MenuItem("Rename", nullptr, nullptr, editorLayer->isOneSelected)) {
				Luxia::GUID e_guid = *editorLayer->selected_assets.begin();
				auto it = asset_view->asset_parent_folders.find(e_guid);
				if (it != asset_view->asset_parent_folders.end()) {
					openRenamePopup = true;
				}
			}

			if (ImGui::MenuItem("Delete", nullptr, nullptr, editorLayer->areMultipleSelected || editorLayer->isOneSelected)) {
				Luxia::GUID e_guid = *editorLayer->selected_assets.begin();
				auto it = asset_view->asset_parent_folders.find(e_guid);
				if (it != asset_view->asset_parent_folders.end()) {
					// Rename Stuff
				}
			}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Import", nullptr, nullptr, editorLayer->areNoneSelected)) {
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

			if (ImGui::BeginMenu("Create", editorLayer->areNoneSelected)) {
				// Should be created in the folder you are in
				if (ImGui::MenuItem("New Folder")) {
					if (std::filesystem::exists(asset_view->selected_folder / "NewFolder")) return;
					std::filesystem::create_directories(asset_view->selected_folder / "NewFolder");
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Material")) {
					if (!asset_view->selected_folder.empty() && std::filesystem::exists(asset_view->selected_folder)) {
						auto mat = editorLayer->GetAssetManager()->CreateAssetFile<Luxia::AssetType::MaterialType>(asset_view->selected_folder, false, "NewMaterial");
						asset_view->RefreshAPFs(editorLayer);
					}
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Scene")) {
					if (!asset_view->selected_folder.empty() && std::filesystem::exists(asset_view->selected_folder)) {
						auto scene = editorLayer->GetAssetManager()->CreateAssetFile<Luxia::AssetType::SceneType>(asset_view->selected_folder, false, "NewScene");
						editorLayer->GetSceneManager()->scene_files.push_back(editorLayer->GetAssetManager()->GetAssetFile<Luxia::Assets::SceneFile>(scene));
						asset_view->RefreshAPFs(editorLayer);
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}


		// Rename Popup
		if (openRenamePopup) {
			ImGui::OpenPopup("Rename Asset Popup");
			openRenamePopup = false;
		}

		static char nameBuffer[256];
		static bool init = false;

		if (ImGui::BeginPopupModal("Rename Asset Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			auto guid = *editorLayer->selected_assets.begin();

			if (!init) {
				auto& asset = editorLayer->GetAssetManager()->GetAssetPool().at(guid);
				strcpy_s(nameBuffer, asset->name.c_str());
				init = true;
			}

			ImGui::InputText("New Name", nameBuffer, sizeof(nameBuffer));

			// Should also rename the actual file on disk, not just the asset name
			if (ImGui::Button("Rename")) {
				std::string namestr = std::string(nameBuffer);
				editorLayer->GetAssetManager()->RenameAsset(guid, namestr);
				asset_view->RefreshAPFs(editorLayer);
				init = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")) {
				init = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsWindowHovered()) {
			if (Luxia::Input::IsMouseButtonJustReleased(LX_MOUSE_BUTTON_4)) {
				if (asset_view->selected_folder != editorLayer->GetAssetManager()->GetAssetDir()) {
					asset_view->selected_folder = asset_view->selected_folder.parent_path();
				}
			}
		}
	}
}