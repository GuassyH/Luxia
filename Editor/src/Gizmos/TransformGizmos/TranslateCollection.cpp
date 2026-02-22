#include "TranslateCollection.h";
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {

	/// Translate (One Axis)
	void TranslateAxis::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::arrowMesh, normalMat);
	}

	bool TranslateAxis::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	void TranslateAxis::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		auto& ent = scene->runtime_entities.find(*editorLayer->selected_assets.begin())->second;
		glm::vec3 cam_to_entity = camera->transform->world_position + (glm::normalize(ent.transform->world_position - camera->transform->world_position) * 15.0f);

		target_transform = ent.transform;

		transform->local_position = cam_to_entity;
		transform->local_rotation = ent.transform->world_rotation;

		transform->AddEulerAngles(rot_offset, true);
		transform->UpdateMatrix();
		editorLayer->physicsWorld->jphSystem.GetBodyInterface().SetPositionAndRotation(
			transform->GetComponent<Luxia::Components::RigidBody>().body->GetID(),
			Luxia::Physics::ToJolt(transform->world_position),
			Luxia::Physics::ToJolt(transform->world_rotation),
			JPH::EActivation::Activate);
	}
	bool TranslateAxis::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active)
			if (scene->runtime_entities.contains(*editorLayer->selected_assets.begin()))
				return true;
		return false;
	}
	void TranslateAxis::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void TranslateAxis::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void TranslateAxis::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}

	void TranslateAxis::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		sceneViewport->active_gizmo_part = this;

		if (!target_transform || !hit.hit) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		// Get on_click_length;
		glm::vec3 translate_direction;

		switch (axis) {
		case Editor::Gizmos::x:
			translate_direction = target_transform->right;
			break;
		case Editor::Gizmos::y:
			translate_direction = target_transform->up;
			break;
		case Editor::Gizmos::z:
			translate_direction = -target_transform->forward;
			break;
		default:
			translate_direction = glm::vec3(0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::ClosestPointOnLineToRay(target_transform->world_position, translate_direction, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		float length = glm::dot(new_hit_vec, translate_direction);
		on_click_length = length;

		is_clicked = true;
		last_hit_vec = new_hit_vec;
	}
	void TranslateAxis::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (!is_clicked) return;

		if (!target_transform) {
			LX_ERROR("Gizmo has no target");
			return;
		}


		glm::vec3 translate_direction;
		switch (axis) {
		case Editor::Gizmos::x:
			translate_direction = target_transform->right;
			break;
		case Editor::Gizmos::y:
			translate_direction = target_transform->up;
			break;
		case Editor::Gizmos::z:
			translate_direction = -target_transform->forward;
			break;
		default:
			translate_direction = glm::vec3(0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::ClosestPointOnLineToRay(target_transform->world_position, translate_direction, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		// if dragged
		if (glm::length2(new_hit_vec - last_hit_vec) > 1e-6f) {
			float length = glm::dot(new_hit_vec, translate_direction);
			target_transform->transform->local_position += translate_direction * (length - on_click_length);
		}
		// Compare the two 
		last_hit_vec = new_hit_vec;
	}
	void TranslateAxis::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}


	/// Translate Two Axis
	void TranslateBiAxis::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::biAxisPlane, normalMat);
	}

	bool TranslateBiAxis::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	void TranslateBiAxis::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		auto& ent = scene->runtime_entities.find(*editorLayer->selected_assets.begin())->second;
		glm::vec3 cam_to_entity = camera->transform->world_position + (glm::normalize(ent.transform->world_position - camera->transform->world_position) * 15.0f);

		target_transform = ent.transform;

		transform->local_position = cam_to_entity;
		transform->local_rotation = ent.transform->world_rotation;

		transform->AddEulerAngles(rot_offset, true);
		transform->UpdateMatrix();
		editorLayer->physicsWorld->jphSystem.GetBodyInterface().SetPositionAndRotation(
			transform->GetComponent<Luxia::Components::RigidBody>().body->GetID(),
			Luxia::Physics::ToJolt(transform->world_position),
			Luxia::Physics::ToJolt(transform->world_rotation),
			JPH::EActivation::Activate);
	}
	bool TranslateBiAxis::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active)
			if (scene->runtime_entities.contains(*editorLayer->selected_assets.begin()))
				return true;
		return false;
	}
	void TranslateBiAxis::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void TranslateBiAxis::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void TranslateBiAxis::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}
	void TranslateBiAxis::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		sceneViewport->active_gizmo_part = this;

		if (!target_transform || !hit.hit) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		// Get on_click_length;
		glm::vec3 axis_forward;
		glm::vec3 axis_up;
		switch (axis) {
		case Editor::Gizmos::xy:
			axis_forward = target_transform->right;
			axis_up = target_transform->up;
			break;
		case Editor::Gizmos::xz:
			axis_forward = target_transform->right;
			axis_up = target_transform->forward;
			break;
		case Editor::Gizmos::yz:
			axis_forward = target_transform->forward;
			axis_up = target_transform->up;
			break;
		default:
			axis_forward = glm::vec3(0.0f, 0.0f, -1.0f);
			axis_up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::RayIntersectPlane(target_transform->world_position, axis_forward, axis_up, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_pos = hit_pos;

		is_clicked = true;
		last_intersect_pos = new_hit_pos;
	}
	void TranslateBiAxis::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (!is_clicked) return;

		if (!target_transform) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		glm::vec3 axis_forward;
		glm::vec3 axis_up;
		switch (axis) {
		case Editor::Gizmos::xy:
			axis_forward = target_transform->right;
			axis_up = target_transform->up;
			break;
		case Editor::Gizmos::xz:
			axis_forward = target_transform->right;
			axis_up = target_transform->forward;
			break;
		case Editor::Gizmos::yz:
			axis_forward = target_transform->forward;
			axis_up = target_transform->up;
			break;
		default:
			axis_forward = glm::vec3(0.0f, 0.0f, -1.0f);
			axis_up = glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		}

		glm::vec3 hit_pos;
		Luxia::Physics::RayIntersectPlane(target_transform->world_position, axis_forward, axis_up, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_pos = hit_pos;

		// if dragged
		glm::vec3 delta = new_hit_pos - last_intersect_pos;
		if (glm::length2(delta) > 1e-6f) {
			target_transform->local_position += delta;
		}
		// Compare the two 
		last_intersect_pos = new_hit_pos;
	}
	void TranslateBiAxis::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}



	// Create the collection itself
	TranslateCollection::TranslateCollection(entt::registry* reg) {
		/// Arrows (One Axis)
		entt::entity arrow_x_ent = reg->create();
		auto& arrow_x_t = reg->emplace<Luxia::Components::Transform>(arrow_x_ent);

		arrow_x_t.ent_id = arrow_x_ent;
		arrow_x_t.ent_guid = Luxia::GUID(0);
		arrow_x_t.reg = reg;
		arrow_x_t.transform = &arrow_x_t;

		auto arrow_x_gizmo_part = std::make_shared<Gizmos::TranslateAxis>();
		arrow_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		arrow_x_gizmo_part->transform = &arrow_x_t;
		arrow_x_gizmo_part->rot_offset = glm::vec3(0.0f, 90.0f, 0.0f);
		arrow_x_gizmo_part->axis = Axis::x;
		arrow_x_gizmo_part->OnInit();

		auto& arrow_x_gizmo_behaviour = arrow_x_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_x_gizmo_behaviour.gizmo_part = arrow_x_gizmo_part;

		// Y ARROW
		entt::entity arrow_y_ent = reg->create();
		auto& arrow_y_t = reg->emplace<Luxia::Components::Transform>(arrow_y_ent);

		arrow_y_t.ent_id = arrow_y_ent;
		arrow_y_t.ent_guid = Luxia::GUID(0);
		arrow_y_t.reg = reg;
		arrow_y_t.transform = &arrow_y_t;

		auto arrow_y_gizmo_part = std::make_shared<Gizmos::TranslateAxis>();
		arrow_y_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		arrow_y_gizmo_part->transform = &arrow_y_t;
		arrow_y_gizmo_part->rot_offset = glm::vec3(-90.0f, 0.0f, 0.0f);
		arrow_y_gizmo_part->axis = Axis::y;
		arrow_y_gizmo_part->OnInit();

		auto& arrow_y_gizmo_behaviour = arrow_y_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_y_gizmo_behaviour.gizmo_part = arrow_y_gizmo_part;

		// Z ARROW
		entt::entity arrow_z_ent = reg->create();
		auto& arrow_z_t = reg->emplace<Luxia::Components::Transform>(arrow_z_ent);

		arrow_z_t.ent_id = arrow_z_ent;
		arrow_z_t.ent_guid = Luxia::GUID(0);
		arrow_z_t.reg = reg;
		arrow_z_t.transform = &arrow_z_t;

		auto arrow_z_gizmo_part = std::make_shared<Gizmos::TranslateAxis>();
		arrow_z_gizmo_part->normalMat = Gizmos::GizmoResources::zMaterial;
		arrow_z_gizmo_part->transform = &arrow_z_t;
		arrow_z_gizmo_part->rot_offset = glm::vec3(0.0f, 0.0f, 0.0f);
		arrow_z_gizmo_part->axis = Axis::z;
		arrow_z_gizmo_part->OnInit();

		auto& arrow_z_gizmo_behaviour = arrow_z_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_z_gizmo_behaviour.gizmo_part = arrow_z_gizmo_part;

		// RigidBodies
		arrow_x_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& arrow_x_col = arrow_x_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		arrow_x_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		arrow_x_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		arrow_x_col.InitCollider();

		arrow_y_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& arrow_y_col = arrow_y_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		arrow_y_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		arrow_y_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		arrow_y_col.InitCollider();

		arrow_z_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& arrow_z_col = arrow_z_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		arrow_z_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		arrow_z_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		arrow_z_col.InitCollider();

		/// Planes (Two Axis)
		entt::entity plane_xz_ent = reg->create();
		auto& plane_xz_t = reg->emplace<Luxia::Components::Transform>(plane_xz_ent);

		plane_xz_t.ent_id = plane_xz_ent;
		plane_xz_t.ent_guid = Luxia::GUID(0);
		plane_xz_t.reg = reg;
		plane_xz_t.transform = &plane_xz_t;

		auto plane_xz_gizmo_part = std::make_shared<Gizmos::TranslateBiAxis>();
		plane_xz_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		plane_xz_gizmo_part->transform = &plane_xz_t;
		plane_xz_gizmo_part->rot_offset = glm::vec3(90.0f, 0.0f, 0.0f);
		plane_xz_gizmo_part->transform->local_scale = glm::vec3(1.0f);
		plane_xz_gizmo_part->axis = Axis::xz;
		plane_xz_gizmo_part->OnInit();

		auto& plane_xz_gizmo_behaviour = plane_xz_t.AddComponent<Gizmos::GizmoBehaviour>();
		plane_xz_gizmo_behaviour.gizmo_part = plane_xz_gizmo_part;

		// xy
		entt::entity plane_xy_ent = reg->create();
		auto& plane_xy_t = reg->emplace<Luxia::Components::Transform>(plane_xy_ent);

		plane_xy_t.ent_id = plane_xy_ent;
		plane_xy_t.ent_guid = Luxia::GUID(0);
		plane_xy_t.reg = reg;
		plane_xy_t.transform = &plane_xy_t;

		auto plane_xy_gizmo_part = std::make_shared<Gizmos::TranslateBiAxis>();
		plane_xy_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		plane_xy_gizmo_part->transform = &plane_xy_t;
		plane_xy_gizmo_part->rot_offset = glm::vec3(0.0f, 0.0f, 0.0f);
		plane_xy_gizmo_part->transform->local_scale = glm::vec3(1.0f);
		plane_xy_gizmo_part->axis = Axis::xy;
		plane_xy_gizmo_part->OnInit();

		auto& plane_xy_gizmo_behaviour = plane_xy_t.AddComponent<Gizmos::GizmoBehaviour>();
		plane_xy_gizmo_behaviour.gizmo_part = plane_xy_gizmo_part;

		// yz
		entt::entity plane_yz_ent = reg->create();
		auto& plane_yz_t = reg->emplace<Luxia::Components::Transform>(plane_yz_ent);

		plane_yz_t.ent_id = plane_yz_ent;
		plane_yz_t.ent_guid = Luxia::GUID(0);
		plane_yz_t.reg = reg;
		plane_yz_t.transform = &plane_yz_t;

		auto plane_yz_gizmo_part = std::make_shared<Gizmos::TranslateBiAxis>();
		plane_yz_gizmo_part->normalMat = Gizmos::GizmoResources::zMaterial;
		plane_yz_gizmo_part->transform = &plane_yz_t;
		plane_yz_gizmo_part->rot_offset = glm::vec3(0.0f, -90.0f, 0.0f);
		plane_yz_gizmo_part->transform->local_scale = glm::vec3(1.0f);
		plane_yz_gizmo_part->axis = Axis::yz;
		plane_yz_gizmo_part->OnInit();

		auto& plane_yz_gizmo_behaviour = plane_yz_t.AddComponent<Gizmos::GizmoBehaviour>();
		plane_yz_gizmo_behaviour.gizmo_part = plane_yz_gizmo_part;


		// RigidBodies
		plane_xz_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& plane_xz_col = plane_xz_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		plane_xz_col.scale = glm::vec3(0.5f, 0.5f, 0.1f);
		plane_xz_col.offset = glm::vec3(0.75f, 0.75f, 0.0f);
		plane_xz_col.InitCollider();

		plane_xy_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& plane_xy_col = plane_xy_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		plane_xy_col.scale = glm::vec3(0.5f, 0.5f, 0.1f);
		plane_xy_col.offset = glm::vec3(0.75f, 0.75f, 0.0f);
		plane_xy_col.InitCollider();

		plane_yz_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& plane_yz_col = plane_yz_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		plane_yz_col.scale = glm::vec3(0.5f, 0.5f, 0.1f);
		plane_yz_col.offset = glm::vec3(0.75f, 0.75f, 0.0f);
		plane_yz_col.InitCollider();

		// Push to collection
		// Arrows
		behaviours.push_back(arrow_x_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(arrow_y_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(arrow_z_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		// Plane
		behaviours.push_back(plane_xz_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(plane_xy_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(plane_yz_t.TryGetComponent<Gizmos::GizmoBehaviour>());
	}
}