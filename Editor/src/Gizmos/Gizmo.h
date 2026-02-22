#pragma once

#include <unordered_map>
#include "Luxia/Core/GUID.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Entity.h"
#include "Luxia/Components/ComponentRegistry.h"
#include "Luxia/Managers/ResourceManager.h"

#include "Luxia/Components/Component.h"
#include "Luxia/Components/Camera.h"
#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Rendering/API/IRenderer.h"

#include "EditorLayer.h"

/// <summary>
/// An entity will hold a GizmoBehaviour, which is given an abstracted unique pointer to a GizmoPart
/// For example for translate there will be three entities, one will have x arrow, one y arrow, and one z arrow
/// </summary>

namespace Editor::Panels {
	class SceneViewport;
}

namespace Editor::Gizmos {

	enum Axis {
		x = 0b0,
		y = 0b1 << 0,
		z = 0b1 << 1,
		xy = 0b1 << 2,
		xz = 0b1 << 3,
		yz = 0b1 << 4,
	};

	class GizmoResources {
	public:
		static std::shared_ptr<Luxia::IMaterial> xMaterial;
		static std::shared_ptr<Luxia::IMaterial> yMaterial;
		static std::shared_ptr<Luxia::IMaterial> zMaterial;
		static std::shared_ptr<Luxia::IMaterial> orgioMaterial;
		static std::shared_ptr<Luxia::IMaterial> hoverMaterial;

		static std::shared_ptr<Luxia::Mesh> arrowMesh;
		static std::shared_ptr<Luxia::Mesh> scaleMesh;
		static std::shared_ptr<Luxia::Mesh> rotateQMesh; // Quadrant Mesh
		static std::shared_ptr<Luxia::Mesh> rotateWMesh; // Wheel Mesh
		static std::shared_ptr<Luxia::Mesh> biAxisPlane; // Wheel Mesh

		static void Init(std::filesystem::path path_to_gizmos);
	};

	// The Gizmo Part
	class GizmoPart {
	public:  
		Luxia::Components::Transform* transform = nullptr;

		bool is_active = true;

		virtual ~GizmoPart() = default;
		virtual void OnInit() = 0;


		// Should it be updated and what should it do when updated
		virtual bool ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;
		virtual void OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;

		// Should it be render and what should it do when rendering
		virtual bool ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;
		virtual void OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) = 0;
		
		// What to do when hovering
		virtual void OnHover() = 0;
		virtual void OnUnhover() = 0;

		// What to do when Clicking, Dragging, or letting go of click
		virtual void OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) = 0;
		virtual void OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) = 0;
		virtual void OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) = 0;
	};

	// The Component
	class GizmoBehaviour : public Luxia::Components::Component {
	public:
		std::shared_ptr<GizmoPart> gizmo_part = nullptr;
		GizmoBehaviour() = default;
	};

	// The Collection
	struct GizmoCollection {
		std::vector<GizmoBehaviour*> behaviours;
	};
};