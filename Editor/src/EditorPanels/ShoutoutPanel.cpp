#include "ShoutoutPanel.h"
#include "EditorLayer.h"

#include <windows.h>
#include <shellapi.h>

namespace Editor::Panels {

	static std::shared_ptr<Luxia::ITexture> Redston4Dpfp = Luxia::Platform::Assets::CreateTexture();
	static std::shared_ptr<Luxia::ITexture> Copyrightpfp = Luxia::Platform::Assets::CreateTexture();
	
	void ShoutoutPanel::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Shoutout Panel: Init");

		Redston4Dpfp->LoadFromFile("C:/dev/Luxia/Editor/resources/shoutouts/redston4d.png");
		Copyrightpfp->LoadFromFile("C:/dev/Luxia/Editor/resources/shoutouts/copyright.png");
	}


	void ShoutoutPanel::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Shoutout Panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Shoutout Panel";
		TabPopupContextMenu();

		ImGui::Separator();

		// Redston4D
		ImGui::TextColored(ImVec4(71.0f / 255.0f, 73.0f / 255.0f, 1.0f, 1.0f), "Redston4D");
		ImGui::Image((ImTextureRef)Redston4Dpfp->texID, ImVec2(50, 50));
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();

			ImGui::TextColored(ImVec4(221.0f / 255.0f, 90.0f / 255.0f, 19.0f / 255.0f, 1.0f), "Image owned by Redston4D");

			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		ImGui::TextWrapped("For his charitable contributions in boosting morale and marketing (adding the Luxia logo as a cosmetic in his upcoming game)\nWishlist CarBrix!");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();

			ImGui::TextColored(ImVec4(221.0f / 255.0f, 90.0f / 255.0f, 19.0f / 255.0f, 1.0f), "Click to Wishlist CarBrix now!");

			if (Luxia::Input::IsMouseButtonJustPressed(LX_MOUSE_BUTTON_1)) {
				ShellExecute(0, 0, L"https://store.steampowered.com/app/3712430/Carbrix/", 0, 0, SW_SHOW);
			}

			ImGui::EndTooltip();
		}

		ImGui::Separator();
		
		ImGui::End();
	}
	void ShoutoutPanel::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		Redston4Dpfp->Delete();
	}

	void ShoutoutPanel::OnEvent(Luxia::Event& e) {
		// Luxia::EventDispatcher dispatcher(e);
	}
}