#include "Gizmo.h"
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {


	void TranslatePart::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::arrowMesh, normalMat);
	}

	bool TranslatePart::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	void TranslatePart::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	bool TranslatePart::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active) return true;
		else return false;
	}
	void TranslatePart::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void TranslatePart::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void TranslatePart::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}

	void TranslatePart::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
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
	void TranslatePart::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
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
	void TranslatePart::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}

	TranslateCollection::TranslateCollection(entt::registry* reg) {
		// X ARROW
		entt::entity arrow_x_ent = reg->create();
		auto& arrow_x_t = reg->emplace<Luxia::Components::Transform>(arrow_x_ent);

		arrow_x_t.ent_id = arrow_x_ent;
		arrow_x_t.ent_guid = Luxia::GUID(0);
		arrow_x_t.reg = reg;
		arrow_x_t.transform = &arrow_x_t;

		auto arrow_x_gizmo_part = std::make_shared<Gizmos::TranslatePart>();
		arrow_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		arrow_x_gizmo_part->transform = &arrow_x_t;
		arrow_x_gizmo_part->rot = glm::vec3(0.0f, 90.0f, 0.0f);
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

		auto arrow_y_gizmo_part = std::make_shared<Gizmos::TranslatePart>();
		arrow_y_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		arrow_y_gizmo_part->transform = &arrow_y_t;
		arrow_y_gizmo_part->rot = glm::vec3(-90.0f, 0.0f, 0.0f);
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

		auto arrow_z_gizmo_part = std::make_shared<Gizmos::TranslatePart>();
		arrow_z_gizmo_part->normalMat = Gizmos::GizmoResources::zMaterial;
		arrow_z_gizmo_part->transform = &arrow_z_t;
		arrow_z_gizmo_part->rot = glm::vec3(0.0f, 0.0f, 0.0f);
		arrow_z_gizmo_part->axis = Axis::z;
		arrow_z_gizmo_part->OnInit();

		auto& arrow_z_gizmo_behaviour = arrow_z_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_z_gizmo_behaviour.gizmo_part = arrow_z_gizmo_part;

		// RigidBodies
		arrow_x_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& x_col = arrow_x_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		x_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		x_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		x_col.InitCollider();

		arrow_y_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& y_col = arrow_y_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		y_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		y_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		y_col.InitCollider();

		arrow_z_t.AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
		auto& z_col = arrow_z_t.AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
		z_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
		z_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
		z_col.InitCollider();

		// Push to collection
		behaviours.push_back(arrow_x_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(arrow_y_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		behaviours.push_back(arrow_z_t.TryGetComponent<Gizmos::GizmoBehaviour>());
	}

}