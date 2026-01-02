#pragma once

#include "IEditorPanel.h"

namespace Editor::Panels {
	class Profiler : public Editor::IEditorPanel {
	public:
		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;


		virtual void OnEvent(Luxia::Event& e) override;
	};
}
