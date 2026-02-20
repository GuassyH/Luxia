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
/// 
/// </summary>

namespace Editor::Gizmos {

	class GizmoResources {
	public:
		static std::shared_ptr<Luxia::IMaterial> xMaterial;
		static std::shared_ptr<Luxia::IMaterial> yMaterial;
		static std::shared_ptr<Luxia::IMaterial> zMaterial;
		static std::shared_ptr<Luxia::IMaterial> orgioMaterial;
		static std::shared_ptr<Luxia::IMaterial> hoverMaterial;

		static std::shared_ptr<Luxia::Mesh> arrowMesh;

		static void Init(std::filesystem::path path_to_gizmos);
	};

	// The Gizmo Part
	class GizmoPart {
	public:  
		Luxia::Components::Transform* transform = nullptr;

		virtual ~GizmoPart() = default;
		virtual void OnInit() = 0;


		virtual bool ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;
		virtual void OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;

		virtual bool ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) = 0;
		virtual void OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) = 0;
		
		virtual void OnHover() = 0;
		virtual void OnUnhover() = 0;

		virtual void OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer) = 0;
		virtual void OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer) = 0;
	};

	// Arrow Part (moves the
	class ArrowPart : public GizmoPart {
	public:
		std::shared_ptr<Luxia::IMaterial> normalMat = nullptr;
		std::shared_ptr<Luxia::IMaterial> hoverMat = nullptr;
		Luxia::Components::MeshRenderer* mesh_renderer = nullptr;

		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 responsible_axis = glm::vec3(0.0f);
		glm::vec3 last_hit_vec = glm::vec3(0.0f);
		float on_click_length = 0.0f;

		Luxia::Components::Transform* target_transform = nullptr;

		bool is_clicked = false;
		bool is_hovered = false;
		bool is_active = true;

		virtual void OnInit() override;
		virtual bool ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual void OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual bool ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) override;
		virtual void OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) override;
		virtual void OnUnhover() override;
		virtual void OnHover() override;
		virtual void OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer) override;
		virtual void OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer) override;
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


	GizmoCollection ArrowCollection(entt::registry* reg);
};