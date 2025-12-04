#pragma once

#include "IEditorPanel.h"

namespace Talloren::Editor::Panel {
	class HierarchyPanel : public Talloren::Editor::IEditorPanel {
	public:
		virtual void Init() override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload() override;

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
