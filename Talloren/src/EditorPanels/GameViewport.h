#pragma once

#include "IEditorPanel.h"

namespace Talloren::Editor::Panel {
	class GameViewport : public Talloren::Editor::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload() override;

		WeakPtrProxy<Luxia::ITexture> output_texture;

		bool RenderImage(Luxia::RenderCameraEvent& e);

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
