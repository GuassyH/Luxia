#pragma once

#include "IEditorPanel.h"


namespace Talloren::Editor::Panel {
	class HierarchyPanel : public Talloren::Editor::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload() override;

		void DrawEntitySelectable(Luxia::Components::Transform* entity, Talloren::Layers::EditorLayer* editorLayer, int level);

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
