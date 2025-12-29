#include "AssetView.h"

#include "EditorLayer.h"
#include "EditorScripts/SceneCameraScript.h"

#include "AssetViewPanels/AssetFolderTree.h"
#include "AssetViewPanels/AssetViewer.h"



namespace Talloren::Panels {
	static AssetFolderTree astfoldertree;
	static AssetViewer astviewer;

	void AssetView::RefreshAPFs(Talloren::Layers::EditorLayer* editorLayer, bool reset_dir) {
		asset_parent_folders.clear();

		// Go through each assetfile, then each asset in the assetfile, set its parent folder
		for (auto& [guid, assetfile] : editorLayer->GetAssetManager()->GetAssetFilePool()) {
			if (assetfile) {
				const auto parent = assetfile->assetPath.parent_path(); // Temp cache parent path
				for (auto& asset : assetfile->assets) {
					if (!asset) continue;
					asset_parent_folders[asset->guid] = parent;
				}
			}
		}

		// Reset selected folder to asset dir if requested
		if(reset_dir)
			selected_folder = editorLayer->GetAssetManager()->GetAssetDir();

		LX_INFO("AssetView: Refreshed Asset Parent Folders");
	}

	bool queued_for_refresh = false;
	// CORE
	void AssetView::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - AssetView Panel: Init");
		RefreshAPFs(editorLayer, true);
		astviewer.Init();
	}

	void AssetView::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGuiWindowClass assetViewWindowClass;
		assetViewWindowClass.ClassId = ImGui::GetID("AssetView_WindowClass");
		assetViewWindowClass.DockingAllowUnclassed = false;

		ImGui::SetNextWindowClass(&assetViewWindowClass);
		bool const shouldDrawWindowContents = ImGui::Begin("Asset View");

		std::shared_ptr<Luxia::AssetManager> asset_manager = editorLayer->GetAssetManager();

		#pragma region Dockspace
		ImGuiID const dockspaceID = ImGui::GetID("AssetView_Dockspace");
		ImGuiWindowClass assetViewClass;
		assetViewClass.ClassId = dockspaceID;
		assetViewClass.DockingAllowUnclassed = false;

		static bool asset_dock_built = false;
		if (!asset_dock_built) {
			ImGui::DockBuilderRemoveNode(dockspaceID); 
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
			ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->WorkSize);

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
			asset_dock_built = true;
		}
		#pragma endregion

		// ImGuiDockNodeFlags const dockFlags = shouldDrawWindowContents ? ImGuiDockNodeFlags_None : ImGuiDockNodeFlags_KeepAliveOnly;
		ImGui::DockSpace(dockspaceID, ImGui::GetContentRegionAvail(), ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton, &assetViewClass);

		std::unordered_map<Luxia::GUID, WeakPtrProxy<Luxia::Assets::Asset>> assets_to_draw = {};

		if(queued_for_refresh) {
			RefreshAPFs(editorLayer, false);
			queued_for_refresh = false;
		}

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
			
		ImGui::End();
	}

	
	void AssetView::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
	
	}

	void AssetView::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::MessageSentEvent>([this](Luxia::MessageSentEvent& e_msg) {
			if (e_msg.GetMessageStr() == "Asset Created" || e_msg.GetMessageStr() == "Asset Deleted") {
				queued_for_refresh = true;
			}
			return false; // Dont Consume
			});
	}
}
