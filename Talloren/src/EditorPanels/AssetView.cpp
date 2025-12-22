#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

#include "AssetViewPanels/AssetFolderTree.h"
#include "AssetViewPanels/AssetViewer.h"



namespace Talloren::Panels {
	static AssetFolderTree astfoldertree;
	static AssetViewer astviewer;

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

		selected_folder = editorLayer->GetAssetManager()->GetAssetDir();
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
			assets_to_draw = astfoldertree.DrawFolderHierarchy(editorLayer, this);
			ImGui::End();
		}

		if (ImGui::Begin("RightWindow")) {
			ImGui::Text("Asset View"); ImGui::Separator();
			astviewer.DrawAssetFiles(editorLayer, this, assets_to_draw);
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
