#include "GameViewport.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void GameViewport::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - GameView Panel: Init");
	}
	void GameViewport::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Game View");

		if (!output_texture) { ImGui::End(); return; }

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 windowPos = ImGui::GetWindowPos(); // top-left of the window in screen coordinates

		// Camera aspect ratio (width / height)
		float cameraAspect = (float)output_texture->GetWidth() / (float)output_texture->GetHeight();
		float windowAspect = windowSize.x / windowSize.y;

		ImVec2 imageSize;

		if (windowAspect > cameraAspect) {
			// Window is wider than camera -> match height
			imageSize.y = windowSize.y;
			imageSize.x = windowSize.y * cameraAspect;
		}
		else {
			// Window is taller than camera -> match width
			imageSize.x = windowSize.x;
			imageSize.y = windowSize.x / cameraAspect;
		}

		ImVec2 imagePosInWindow;
		imagePosInWindow.x = ((windowSize.x - imageSize.x) * 0.5f) + ImGui::GetCursorPosX();
		imagePosInWindow.y = ((windowSize.y - imageSize.y) * 0.5f) + ImGui::GetCursorPosY();

		ImGui::SetCursorPosX(imagePosInWindow.x);
		ImGui::SetCursorPosY(imagePosInWindow.y);

		ImGui::Image((ImTextureID)(intptr_t)output_texture->texID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

		output_texture = nullptr;

		ImGui::End();
	}
	void GameViewport::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	bool GameViewport::RenderImage(Luxia::RenderCameraEvent& e) {
		if (!e.GetTexture()) { return false; }
		if (e.IsEditorCamera()) { return false; }
		if (!e.IsMainCamera()) { return false; }

		output_texture = e.GetTexture();

		return true; // Consume
	}

	void GameViewport::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}