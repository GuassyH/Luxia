#pragma once
#include "Gizmos/Gizmo.h"

namespace Editor::Panels {
	class SceneViewport;
};

namespace Editor::Gizmos {

	// Scale Part (scales the active transform)
	class ScalePart : public GizmoPart {
	public:
		std::shared_ptr<Luxia::IMaterial> normalMat = nullptr;
		std::shared_ptr<Luxia::IMaterial> hoverMat = nullptr;
		Luxia::Components::MeshRenderer* mesh_renderer = nullptr;

		Axis axis = Axis::x;
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 last_hit_vec = glm::vec3(0.0f);
		float on_click_length = 0.0f;

		Luxia::Components::Transform* target_transform = nullptr;

		bool is_clicked = false;
		bool is_hovered = false;
		bool is_switched_active = true; // used for adding and removing rigidbody

		virtual void OnInit() override;
		virtual bool ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual void OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual bool ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual void OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) override;
		virtual void OnUnhover() override;
		virtual void OnHover() override;
		virtual void OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) override;
		virtual void OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) override;
		virtual void OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) override;
	};

	struct ScaleCollection : GizmoCollection {
		ScaleCollection(entt::registry* reg);
	};

};