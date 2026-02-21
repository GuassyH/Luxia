#include "Gizmo.h"
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {
	void RotatePart::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::rotateMesh, normalMat);
	}

	bool RotatePart::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	void RotatePart::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	bool RotatePart::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active) return true;
		else return false;
	}
	void RotatePart::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void RotatePart::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void RotatePart::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}

	// UNIQUE
	void RotatePart::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		sceneViewport->active_gizmo_part = this;

		if (!target_transform || !hit.hit) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		// Get on_click_length;
		glm::vec3 axis_forward;
		glm::vec3 axis_up;
		switch (axis) {
		case Editor::Gizmos::x:
			axis_forward = target_transform->forward;
			axis_up = target_transform->up;
			break;
		case Editor::Gizmos::y:
			axis_forward = target_transform->right;
			axis_up = target_transform->forward;
			break;
		case Editor::Gizmos::z:
			axis_forward = target_transform->right;
			axis_up = target_transform->up;
			break;
		default:
			axis_forward = glm::vec3(0.0f, 0.0f, -1.0f);
			axis_up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::RayIntersectPlane(target_transform->world_position, axis_forward, axis_up, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_pos = hit_pos - target_transform->world_position;

		is_clicked = true;
		last_intersect_pos = new_hit_pos;
	}

	// UNIQUE
	void RotatePart::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (!is_clicked) return;

		if (!target_transform) {
			LX_ERROR("Gizmo has no target");
			return;
		}


		glm::vec3 rotate_axis;
		glm::vec3 axis_forward;
		glm::vec3 axis_up;
		switch (axis) {
		case Editor::Gizmos::x:
			rotate_axis = target_transform->right;
			axis_forward = target_transform->forward;
			axis_up = target_transform->up;
			break;
		case Editor::Gizmos::y:
			rotate_axis = target_transform->up;
			axis_forward = target_transform->right;
			axis_up = target_transform->forward;
			break;
		case Editor::Gizmos::z:
			rotate_axis = target_transform->forward;
			axis_forward = target_transform->right;
			axis_up = target_transform->up;
			break;
		default:
			rotate_axis = glm::vec3(1.0f, 0.0f, 0.0f);
			axis_forward = glm::vec3(0.0f, 0.0f, -1.0f);
			axis_up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::RayIntersectPlane(target_transform->world_position, axis_forward, axis_up, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_pos = hit_pos - target_transform->world_position;

		// if dragged
		if (glm::length2(new_hit_pos - last_intersect_pos) > 1e-6f) {
			glm::vec3 v0 = glm::normalize(last_intersect_pos);
			glm::vec3 v1 = glm::normalize(new_hit_pos);

			// rotate_axis must be normalized and in the SAME SPACE as v0/v1
			float signedAngle = atan2(
				glm::dot(glm::cross(v0, v1), rotate_axis),
				glm::dot(v0, v1)
			);

			float angleDeg = glm::degrees(signedAngle);
			target_transform->AddEulerAngles(rotate_axis * angleDeg, false);
		}
		// Compare the two 
		last_intersect_pos = new_hit_pos;
	}
	void RotatePart::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}



	RotateCollection::RotateCollection(entt::registry* reg) {
		// X ARROW
		entt::entity rotate_x_ent = reg->create();
		auto& rotate_x_t = reg->emplace<Luxia::Components::Transform>(rotate_x_ent);

		rotate_x_t.ent_id = rotate_x_ent;
		rotate_x_t.ent_guid = Luxia::GUID(0);
		rotate_x_t.reg = reg;
		rotate_x_t.transform = &rotate_x_t;

		auto rotate_x_gizmo_part = std::make_shared<Gizmos::RotatePart>();
		rotate_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		rotate_x_gizmo_part->transform = &rotate_x_t;
		rotate_x_gizmo_part->rot = glm::vec3(-90.0f, -90.0f, 0.0f);
		rotate_x_gizmo_part->axis = Axis::x;
		rotate_x_gizmo_part->OnInit();

		auto& rotate_x_gizmo_behaviour = rotate_x_t.AddComponent<Gizmos::GizmoBehaviour>();
		rotate_x_gizmo_behaviour.gizmo_part = rotate_x_gizmo_part;

		// Y ARROW
		entt::entity rotate_y_ent = reg->create();
		auto& rotate_y_t = reg->emplace<Luxia::Components::Transform>(rotate_y_ent);

		rotate_y_t.ent_id = rotate_y_ent;
		rotate_y_t.ent_guid = Luxia::GUID(0);
		rotate_y_t.reg = reg;
		rotate_y_t.transform = &rotate_y_t;

		auto rotate_y_gizmo_part = std::make_shared<Gizmos::RotatePart>();
		rotate_y_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		rotate_y_gizmo_part->transform = &rotate_y_t;
		rotate_y_gizmo_part->rot = glm::vec3(0.0f, 0.0f, 0.0f);
		rotate_y_gizmo_part->axis = Axis::y;
		rotate_y_gizmo_part->OnInit();

		auto& rotate_y_gizmo_behaviour = rotate_y_t.AddComponent<Gizmos::GizmoBehaviour>();
		rotate_y_gizmo_behaviour.gizmo_part = rotate_y_gizmo_part;

		// Z ARROW
		entt::entity rotate_z_ent = reg->create();
		auto& rotate_z_t = reg->emplace<Luxia::Components::Transform>(rotate_z_ent);

		rotate_z_t.ent_id = rotate_z_ent;
		rotate_z_t.ent_guid = Luxia::GUID(0);
		rotate_z_t.reg = reg;
		rotate_z_t.transform = &rotate_z_t;

		auto rotate_z_gizmo_part = std::make_shared<Gizmos::RotatePart>();
		rotate_z_gizmo_part->normalMat = Gizmos::GizmoResources::zMaterial;
		rotate_z_gizmo_part->transform = &rotate_z_t;
		rotate_z_gizmo_part->rot = glm::vec3(-90.0f, 0.0f, 0.0f);
		rotate_z_gizmo_part->axis = Axis::z;
		rotate_z_gizmo_part->OnInit();

		auto& rotate_z_gizmo_behaviour = rotate_z_t.AddComponent<Gizmos::GizmoBehaviour>();
		rotate_z_gizmo_behaviour.gizmo_part = rotate_z_gizmo_part;


		glm::vec3 col_scale = glm::vec3(1.0f, 0.1f, 1.0f);
		glm::vec3 col_offset = glm::vec3(1.0f, 0.0f, 1.0f);

		// RigidBodies
		rotate_x_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& x_col = rotate_x_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		x_col.scale = col_scale;
		x_col.offset = col_offset;
		x_col.InitCollider();

		rotate_y_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& y_col = rotate_y_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		y_col.scale = col_scale;
		y_col.offset = col_offset;
		y_col.InitCollider();

		rotate_z_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& z_col = rotate_z_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		z_col.scale = col_scale;
		z_col.offset = col_offset;
		z_col.InitCollider();

		// Push to collection
		behaviours.push_back(rotate_x_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(rotate_y_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(rotate_z_t.TryGetComponent<Gizmos::GizmoBehaviour>());
	}

};