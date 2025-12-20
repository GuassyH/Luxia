#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

// THIS IS ONLY FOR WINDOWS 
#include <Windows.h>
#include <shellapi.h>
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

		int sizeNeeded = WideCharToMultiByte(
			CP_UTF8, 0,
			filePath.c_str(), (int)filePath.size(),
			nullptr, 0, nullptr, nullptr
		);

		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(
			CP_UTF8, 0,
			filePath.c_str(), (int)filePath.size(),
			result.data(), sizeNeeded,
			nullptr, nullptr
		);

		return result;
	}

	void AssetView::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - AssetView Panel: Init");
	}

	void AssetView::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Asset View");

		std::shared_ptr<Luxia::AssetManager> asset_manager = editorLayer->GetAssetManager();

		// TEMPORARY
		for (auto [guid, asset] : editorLayer->GetAssetManager()->GetAssetPool()) {
			// Display name
			ImGui::Text(asset->name.c_str());
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

		// should be menu
		if (ImGui::BeginPopupContextWindow("Asset Viewer", ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("Import")) {
				// Open Folder, if you choose something of supported type, import correctly
				// ShellExecuteA(NULL, "open", editorLayer->GetAssetManager()->GetAssetDir().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
				std::string fp = OpenFileDialogue();
				std::filesystem::path filepath = fp;
				if (!filepath.empty() && std::filesystem::exists(filepath)) {
					LX_INFO("Selected Path: {}", filepath.string());
					asset_manager->Import(filepath, false, filepath.filename().string());
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginMenu("Create")) {
				// Should be created in the folder you are in
				if (ImGui::MenuItem("Material")) {
					asset_manager->CreateAssetFile<Luxia::AssetType::MaterialType>("materials", true, "NewMaterial");					
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

		ImGui::End();
	}
	void AssetView::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
	}


	void AssetView::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

	}
}


