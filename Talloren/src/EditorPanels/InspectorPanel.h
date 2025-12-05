#pragma once

#include "IEditorPanel.h"

namespace Talloren::Editor::Panel {
	class InspectorPanel : public Talloren::Editor::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
