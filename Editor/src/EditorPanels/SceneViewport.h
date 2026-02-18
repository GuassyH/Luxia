#pragma once

#include "IEditorPanel.h"
#include "Gizmos/Gizmo.h"

namespace Editor::Panels {
	class SceneViewport : public Editor::IEditorPanel {
	public:
		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		void RenderTopBar(Editor::Layers::EditorLayer* editorLayer);

		void InitGizmos(Editor::Layers::EditorLayer* editorLayer);
		void RenderGizmos(Editor::Layers::EditorLayer* editorLayer, Luxia::Scene* scene, std::shared_ptr<Luxia::ITexture> cam_tex);
		bool GizmoRaycasts(Editor::Layers::EditorLayer* editorLayer, Luxia::Scene* scene, glm::vec3 ray_dir);

		std::vector<Gizmos::GizmoCollection> gizmos;

		WeakPtrProxy<Luxia::ITexture> output_texture;
		Luxia::Components::Transform* cam_ent = nullptr;

		bool RenderImage(Luxia::RenderCameraEvent& e);

		virtual void OnEvent(Luxia::Event& e) override;
	};
}
