#include "Gizmo.h"
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {

	void ScalePart::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::scaleMesh, normalMat);
	}

	bool ScalePart::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		bool should_update = false;

		if (editorLayer->isOneSelected && is_active) {
			if (scene->runtime_entities.contains(*editorLayer->selected_assets.begin())) should_update = true;
			else should_update = false;
		}
		else {
			should_update = false;
		}

		/// Makes sure to remove rigidbody if not use

		// Was inactive and now is activated
		if (!is_switched_active && should_update) {
			auto& body_interface = editorLayer->physicsWorld->jphSystem.GetBodyInterface();
			auto& rb = transform->GetComponent<Luxia::Components::RigidBody>();

			if (!body_interface.IsAdded(rb.body->GetID())) {
				body_interface.AddBody(rb.body->GetID(), JPH::EActivation::DontActivate);
			}

			is_switched_active = true;
		}
		// Is active and should now be inactive
		else if (is_switched_active && !should_update) {
			auto& body_interface = editorLayer->physicsWorld->jphSystem.GetBodyInterface();
			auto& rb = transform->GetComponent<Luxia::Components::RigidBody>();

			if (body_interface.IsAdded(rb.body->GetID())) {
				body_interface.RemoveBody(rb.body->GetID());
			}

			is_switched_active = false;
		}

		return should_update;
	}
	void ScalePart::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		auto& ent = scene->runtime_entities.find(*editorLayer->selected_assets.begin())->second;
		glm::vec3 cam_to_entity = camera->transform->world_position + (glm::normalize(ent.transform->world_position - camera->transform->world_position) * 15.0f);

		target_transform = ent.transform;

		transform->local_position = cam_to_entity;
		transform->local_rotation = ent.transform->world_rotation;

		transform->AddEulerAngles(rot, true);
		transform->UpdateMatrix();
		editorLayer->physicsWorld->jphSystem.GetBodyInterface().SetPositionAndRotation(
			transform->GetComponent<Luxia::Components::RigidBody>().body->GetID(),
			Luxia::Physics::ToJolt(transform->world_position),
			Luxia::Physics::ToJolt(transform->world_rotation),
			JPH::EActivation::Activate);
	}
	bool ScalePart::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active) return true;
		else return false;
	}
	void ScalePart::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void ScalePart::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void ScalePart::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}

	void ScalePart::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		sceneViewport->active_gizmo_part = this;

		if (!target_transform || !hit.hit) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		// Get on_click_length;
		glm::vec3 scale_axis;
		glm::vec3 transform_axis;
		switch (axis) {
		case Editor::Gizmos::x:
			scale_axis = glm::vec3(1.0f, 0.0f, 0.0f);
			transform_axis = target_transform->right;
			break;
		case Editor::Gizmos::y:
			scale_axis = glm::vec3(0.0f, 1.0f, 0.0f);
			transform_axis = target_transform->up;
			break;
		case Editor::Gizmos::z:
			scale_axis = glm::vec3(0.0f, 0.0f, -1.0f);
			transform_axis = -target_transform->forward;
			break;
		default:
			scale_axis = glm::vec3(0.0f);
			transform_axis = glm::vec3(0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::ClosestPointOnLineToRay(target_transform->world_position, transform_axis, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		float length = glm::dot(new_hit_vec, transform_axis);
		on_click_length = length;

		is_clicked = true;
		last_hit_vec = new_hit_vec;
	}
	void ScalePart::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (!is_clicked) return;

		if (!target_transform) {
			LX_ERROR("Gizmo has no target");
			return;
		}


		glm::vec3 scale_axis;
		glm::vec3 transform_axis;
		switch (axis) {
		case Editor::Gizmos::x:
			scale_axis = glm::vec3(1.0f, 0.0f, 0.0f);
			transform_axis = target_transform->right;
			break;
		case Editor::Gizmos::y:
			scale_axis = glm::vec3(0.0f, 1.0f, 0.0f);
			transform_axis = target_transform->up;
			break;
		case Editor::Gizmos::z:
			scale_axis = glm::vec3(0.0f, 0.0f, 1.0f);
			transform_axis = -target_transform->forward;
			break;
		default:
			scale_axis = glm::vec3(0.0f);
			transform_axis = glm::vec3(0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::ClosestPointOnLineToRay(target_transform->world_position, transform_axis, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		// if dragged
		if (glm::length2(new_hit_vec - last_hit_vec) > 1e-6f) {
			float length = glm::dot(new_hit_vec, transform_axis);
			target_transform->transform->local_scale += scale_axis * (length - on_click_length) * 2.0f;
			on_click_length = length;
		}
		// Compare the two 
		last_hit_vec = new_hit_vec;
	}
	void ScalePart::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}

	ScaleCollection::ScaleCollection(entt::registry* reg) {
		// X ARROW
		entt::entity scale_x_ent = reg->create();
		auto& scale_x_t = reg->emplace<Luxia::Components::Transform>(scale_x_ent);

		scale_x_t.ent_id = scale_x_ent;
		scale_x_t.ent_guid = Luxia::GUID(0);
		scale_x_t.reg = reg;
		scale_x_t.transform = &scale_x_t;

		auto scale_x_gizmo_part = std::make_shared<Gizmos::ScalePart>();
		scale_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		scale_x_gizmo_part->transform = &scale_x_t;
		scale_x_gizmo_part->rot = glm::vec3(0.0f, 90.0f, 0.0f);
		scale_x_gizmo_part->axis = Axis::x;
		scale_x_gizmo_part->OnInit();

		auto& scale_x_gizmo_behaviour = scale_x_t.AddComponent<Gizmos::GizmoBehaviour>();
		scale_x_gizmo_behaviour.gizmo_part = scale_x_gizmo_part;

		// Y ARROW
		entt::entity scale_y_ent = reg->create();
		auto& scale_y_t = reg->emplace<Luxia::Components::Transform>(scale_y_ent);

		scale_y_t.ent_id = scale_y_ent;
		scale_y_t.ent_guid = Luxia::GUID(0);
		scale_y_t.reg = reg;
		scale_y_t.transform = &scale_y_t;

		auto scale_y_gizmo_part = std::make_shared<Gizmos::ScalePart>();
		scale_y_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		scale_y_gizmo_part->transform = &scale_y_t;
		scale_y_gizmo_part->rot = glm::vec3(-90.0f, 0.0f, 0.0f);
		scale_y_gizmo_part->axis = Axis::y;
		scale_y_gizmo_part->OnInit();

		auto& scale_y_gizmo_behaviour = scale_y_t.AddComponent<Gizmos::GizmoBehaviour>();
		scale_y_gizmo_behaviour.gizmo_part = scale_y_gizmo_part;

		// Z ARROW
		entt::entity scale_z_ent = reg->create();
		auto& scale_z_t = reg->emplace<Luxia::Components::Transform>(scale_z_ent);

		scale_z_t.ent_id = scale_z_ent;
		scale_z_t.ent_guid = Luxia::GUID(0);
		scale_z_t.reg = reg;
		scale_z_t.transform = &scale_z_t;

		auto scale_z_gizmo_part = std::make_shared<Gizmos::ScalePart>();
		scale_z_gizmo_part->normalMat = Gizmos::GizmoResources::zMaterial;
		scale_z_gizmo_part->transform = &scale_z_t;
		scale_z_gizmo_part->rot = glm::vec3(0.0f, 0.0f, 0.0f);
		scale_z_gizmo_part->axis = Axis::z;
		scale_z_gizmo_part->OnInit();

		auto& scale_z_gizmo_behaviour = scale_z_t.AddComponent<Gizmos::GizmoBehaviour>();
		scale_z_gizmo_behaviour.gizmo_part = scale_z_gizmo_part;

		// RigidBodies
		scale_x_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& x_col = scale_x_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		x_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		x_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		x_col.InitCollider();

		scale_y_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& y_col = scale_y_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		y_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		y_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		y_col.InitCollider();

		scale_z_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& z_col = scale_z_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		z_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		z_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		z_col.InitCollider();

		// Push to collection
		behaviours.push_back(scale_x_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(scale_y_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(scale_z_t.TryGetComponent<Gizmos::GizmoBehaviour>());
	}
}