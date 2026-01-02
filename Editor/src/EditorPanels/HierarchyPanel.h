#pragma once

#include "IEditorPanel.h"


namespace Editor::Panels {
	class HierarchyPanel : public Editor::IEditorPanel {
	public:
		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		void DrawEntitySelectable(Luxia::Entity& entity, Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene);

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
