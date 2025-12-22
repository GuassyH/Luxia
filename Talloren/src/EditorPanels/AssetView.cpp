#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

// THIS IS ONLY FOR WINDOWS 
#include <Windows.h>
#include <shobjidl.h>

namespace Talloren::Panel {


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

	void AssetView::DrawAssetFiles(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>>& assets_to_draw) {
		// For each asset in the assets_to_draw map, draw it (will be more polished later)
		for (auto& [guid, asset] : assets_to_draw) {
			if (!asset) continue;

			// Display name
			ImGui::Text(asset->name.c_str());
			if (ImGui::IsItemHovered())
			{
				std::ostringstream astHint; astHint << asset->name << " GUID";
				std::string hint = astHint.str();
				ImGui::BeginTooltip();
				ImGui::TextUnformatted(hint.c_str());
				ImGui::Separator();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), std::to_string(asset->guid).c_str());
				ImGui::EndTooltip();
			}
			ImGui::SameLine();

			// To easily copy GUID
			std::ostringstream label; label << "Copy##AssetCopy" << asset->guid;
			if (ImGui::Button(label.str().c_str())) {
				std::string guidStr = std::to_string(asset->guid);
				const char* cdata = guidStr.c_str();
				const size_t len = guidStr.size() + 1;
				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
				memcpy(GlobalLock(hMem), cdata, len);
				GlobalUnlock(hMem);
				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
				CloseClipboard();
			}
			ImGui::Separator();
		}
		
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
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginMenu("Create")) {
				// Should be created in the folder you are in
				if (ImGui::MenuItem("Material")) {
					if (!selected_folder.empty() && std::filesystem::exists(selected_folder)) {
						editorLayer->GetAssetManager()->CreateAssetFile<Luxia::AssetType::MaterialType>(selected_folder, false, "NewMaterial");
						RefreshAPFs(editorLayer);
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

	// Same Setup as hierarchy inspector
	static void DrawPathSelectable(const std::filesystem::directory_entry& entry, std::filesystem::path& sel_fol) {
		if (!entry.is_directory()) return;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

		if (sel_fol == entry.path()) flags |= ImGuiTreeNodeFlags_Selected;

		std::ostringstream label; label << "../" << entry.path().stem().string().c_str();
		bool open = ImGui::TreeNodeEx(label.str().c_str(), flags);

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			sel_fol = entry.path();
		}

		if (open) {
			for (const auto& subentry : std::filesystem::directory_iterator(entry.path())) {
				DrawPathSelectable(subentry, sel_fol);
			}
			ImGui::TreePop();
		}
	}

	std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> AssetView::DrawFolderHierarchy(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		// Popup
		if (ImGui::BeginPopupContextWindow("Asset Hierarchy Viewer", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Refresh All")) {
				RefreshAPFs(editorLayer);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> assets_to_draw;
		
		if (!selected_folder.empty() && std::filesystem::exists(selected_folder)) {
		
			// Loop through each sub-directory in the projects asset directory
			for (auto& entry : std::filesystem::directory_iterator(editorLayer->GetProjectManager()->GetProjectPath())) {
				DrawPathSelectable(entry, selected_folder);
			}

			// If its a valid folder, go through each asset, and see if its guid in the a_p_f path is the selected folder
			for (auto& [guid, asset] : editorLayer->GetAssetManager()->GetAssetPool()) {
				if (asset) {
					// if its the same condition itll crash
					auto it = asset_parent_folders.find(guid);
					if (it == asset_parent_folders.end())
						continue;

					if (it->second == selected_folder) {
						assets_to_draw[guid] = asset;
					}
				}
			}
		}

		return assets_to_draw;
	}

	// Goes through each assetfile, checks its assets, sets asset parent folder
	void AssetView::RefreshAPFs(Talloren::Layers::EditorLayer* editorLayer) {
		// Holy nested for if statement
		asset_parent_folders.clear();

		for (auto& [guid, assetfile] : editorLayer->GetAssetManager()->GetAssetFilePool()) {
			if (assetfile) {
				for (auto& asset : assetfile->assets) {
					if (asset) {
						asset_parent_folders[asset->guid] = assetfile->assetPath.parent_path();
					}
				}
			}
		}

		LX_INFO("AssetView: Refreshed Asset Parent Folders");
	}


	// CORE
	void AssetView::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - AssetView Panel: Init");
		RefreshAPFs(editorLayer);
	}

	void AssetView::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		bool const shouldDrawWindowContents = ImGui::Begin("Asset View");

		std::shared_ptr<Luxia::AssetManager> asset_manager = editorLayer->GetAssetManager();


		#pragma region Dockspace
		ImGuiID const dockspaceID = ImGui::GetID("AssetView_Dockspace");
		ImGuiWindowClass workspaceWindowClass;
		workspaceWindowClass.ClassId = dockspaceID;
		workspaceWindowClass.DockingAllowUnclassed = false;

		if (!ImGui::DockBuilderGetNode(dockspaceID)) {
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
			ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetContentRegionAvail());

			ImGuiID leftDockID = 0, rightDockID = 0;
			ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.5f, &leftDockID, &rightDockID);

			ImGuiDockNode* pLeftNode = ImGui::DockBuilderGetNode(leftDockID);
			ImGuiDockNode* pRightNode = ImGui::DockBuilderGetNode(rightDockID);
			pLeftNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverMe;
			pRightNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverMe;
			// Dock windows
			ImGui::DockBuilderDockWindow("LeftWindow", leftDockID);
			ImGui::DockBuilderDockWindow("RightWindow", rightDockID);

			ImGui::DockBuilderFinish(dockspaceID);
		}

		ImGuiDockNodeFlags const dockFlags = shouldDrawWindowContents ? ImGuiDockNodeFlags_None : ImGuiDockNodeFlags_KeepAliveOnly;
		ImGui::DockSpace(dockspaceID, ImGui::GetContentRegionAvail(), dockFlags, &workspaceWindowClass);
#pragma endregion

		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> assets_to_draw = {};

		if (ImGui::Begin("LeftWindow")) {
			ImGui::Text("Asset Hierarchy"); ImGui::Separator();
			assets_to_draw = DrawFolderHierarchy(editorLayer, scene);
			ImGui::End();
		}

		if (ImGui::Begin("RightWindow")) {
			ImGui::Text("Asset View"); ImGui::Separator();
			DrawAssetFiles(editorLayer, scene, assets_to_draw);
			ImGui::End();
		}


		// should be menu


		ImGui::End();
	}

	
	void AssetView::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
	
	}

	void AssetView::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

	}
}


