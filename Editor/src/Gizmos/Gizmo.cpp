#include "Gizmo.h"
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {

	static bool closestPointOnLineToRay(
		const glm::vec3& linePoint,
		const glm::vec3& lineDir,   // normalize this
		const glm::vec3& rayOrigin,
		const glm::vec3& rayDir,    // normalize this
		glm::vec3& outClosestPoint
	) {
		glm::vec3 w0 = linePoint - rayOrigin;

		float a = glm::dot(lineDir, lineDir); // = 1
		float b = glm::dot(lineDir, rayDir);
		float c = glm::dot(rayDir, rayDir);   // = 1
		float d = glm::dot(lineDir, w0);
		float e = glm::dot(rayDir, w0);

		float denom = a * c - b * b;

		float t, s;

		// If not parallel
		if (fabs(denom) > 1e-6f) {
			t = (b * e - c * d) / denom;
			s = (a * e - b * d) / denom;
		}
		else {
			// Parallel: pick arbitrary closest
			t = -d / a;
			s = 0.0f;
		}

		// Enforce ray constraint
		if (s < 0.0f) {
			s = 0.0f;
			t = -d / a; // project ray origin onto line
		}

		outClosestPoint = linePoint + t * lineDir;
		return true;
	}


	std::shared_ptr<Luxia::IMaterial> GizmoResources::xMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::yMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::zMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::orgioMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::hoverMaterial = nullptr;

	std::shared_ptr<Luxia::Mesh> GizmoResources::arrowMesh = nullptr;
	std::shared_ptr<Luxia::Mesh> GizmoResources::scaleMesh = nullptr;

	void GizmoResources::Init(std::filesystem::path path_to_gizmos) {
		// Materials
		xMaterial = Luxia::Platform::Assets::CreateMaterial();
		yMaterial = Luxia::Platform::Assets::CreateMaterial();
		zMaterial = Luxia::Platform::Assets::CreateMaterial();
		orgioMaterial = Luxia::Platform::Assets::CreateMaterial();
		hoverMaterial = Luxia::Platform::Assets::CreateMaterial();

		xMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		xMaterial->color = glm::vec4(1.0f, 0.4392f, 0.4784f, 1.0f);

		yMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		yMaterial->color = glm::vec4(0.5569f, 1.0f, 0.5569f, 1.0f);

		zMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		zMaterial->color = glm::vec4(0.1765f, 0.9020f, 1.0f, 1.0f);

		orgioMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		orgioMaterial->color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

		hoverMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		hoverMaterial->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		path_to_gizmos = path_to_gizmos.lexically_normal();
		// Meshes
		std::shared_ptr<Luxia::IModel> arrowModel = Luxia::Platform::Assets::CreateModel();
		arrowModel->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
		arrowMesh = arrowModel->meshes[0];

		std::shared_ptr<Luxia::IModel> scaleModel = Luxia::Platform::Assets::CreateModel();
		scaleModel->LoadFromPath(path_to_gizmos / "scale/scale.fbx");
		scaleMesh = scaleModel->meshes[0];

	
	}


#pragma region Rotate


#pragma endregion 

#pragma region Arrow

	void ArrowPart::OnInit() {
		hoverMat = GizmoResources::hoverMaterial;
		mesh_renderer = &transform->AddComponent<Luxia::Components::MeshRenderer>(GizmoResources::arrowMesh, normalMat);
	}

	bool ArrowPart::ShouldUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	void ArrowPart::OnUpdate(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
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
	bool ArrowPart::ShouldRender(Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera, Luxia::Scene* scene) {
		if (editorLayer->isOneSelected && is_active) return true;
		else return false;
	}
	void ArrowPart::OnRender(Luxia::Rendering::IRenderer* renderer, Editor::Layers::EditorLayer* editorLayer, Luxia::Components::Camera* camera) {
		if (!mesh_renderer) return;
		renderer->RenderMesh(mesh_renderer->mesh.get(), mesh_renderer->material.get(), transform->GetMatrix(), camera->GetCamera()->GetViewMat(), camera->GetCamera()->GetProjMat());
	}
	void ArrowPart::OnUnhover() {
		if (is_hovered) {
			mesh_renderer->material = normalMat;
			is_hovered = false;
		}
	}
	void ArrowPart::OnHover() {
		if (!is_hovered) {
			mesh_renderer->material = hoverMat;
			is_hovered = true;
		}
	}

	void ArrowPart::OnClick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		sceneViewport->active_gizmo_part = this;
		
		if (!target_transform || !hit.hit) {
			LX_ERROR("Gizmo has no target");
			return;
		}

		// Get on_click_length;
		glm::vec3 translate_direction;

		switch (axis){
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
		closestPointOnLineToRay(target_transform->world_position, translate_direction, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		float length = glm::dot(new_hit_vec, translate_direction);
		on_click_length = length;

		is_clicked = true;
		last_hit_vec = new_hit_vec;
	}
	void ArrowPart::OnDrag(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
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
		closestPointOnLineToRay(target_transform->world_position, translate_direction, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		// if dragged
		if (glm::length2(new_hit_vec - last_hit_vec) > 1e-6f) {
			glm::vec3 axisDir = translate_direction;
			float length = glm::dot(new_hit_vec, axisDir);
			target_transform->transform->local_position += translate_direction * (length - on_click_length);
		}
		// Compare the two 
		last_hit_vec = new_hit_vec;
	}
	void ArrowPart::OnUnclick(Luxia::Physics::RayCastHit& hit, Editor::Layers::EditorLayer* editorLayer, Editor::Panels::SceneViewport* sceneViewport) {
		if (is_clicked) {
			sceneViewport->active_gizmo_part = nullptr;
			target_transform = nullptr;
			is_clicked = false;
		}
	}

	ArrowCollection::ArrowCollection(entt::registry* reg) {
		// X ARROW
		entt::entity arrow_x_ent = reg->create();
		auto& arrow_x_t = reg->emplace<Luxia::Components::Transform>(arrow_x_ent);

		arrow_x_t.ent_id = arrow_x_ent;
		arrow_x_t.ent_guid = Luxia::GUID(0);
		arrow_x_t.reg = reg;
		arrow_x_t.transform = &arrow_x_t;

		auto arrow_x_gizmo_part = std::make_shared<Gizmos::ArrowPart>();
		arrow_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		arrow_x_gizmo_part->transform = &arrow_x_t;
		arrow_x_gizmo_part->rot = glm::vec3(0.0f, 90.0f, 0.0f);
		arrow_x_gizmo_part->axis = Axis::x;
;		arrow_x_gizmo_part->OnInit();

		auto& arrow_x_gizmo_behaviour = arrow_x_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_x_gizmo_behaviour.gizmo_part = arrow_x_gizmo_part;

		// Y ARROW
		entt::entity arrow_y_ent = reg->create();
		auto& arrow_y_t = reg->emplace<Luxia::Components::Transform>(arrow_y_ent);

		arrow_y_t.ent_id = arrow_y_ent;
		arrow_y_t.ent_guid = Luxia::GUID(0);
		arrow_y_t.reg = reg;
		arrow_y_t.transform = &arrow_y_t;

		auto arrow_y_gizmo_part = std::make_shared<Gizmos::ArrowPart>();
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

		auto arrow_z_gizmo_part = std::make_shared<Gizmos::ArrowPart>();
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

#pragma endregion


#pragma region Scale

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
		closestPointOnLineToRay(target_transform->world_position, transform_axis, hit.ray.origin, hit.ray.direction, hit_pos);
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
		closestPointOnLineToRay(target_transform->world_position, transform_axis, hit.ray.origin, hit.ray.direction, hit_pos);
		glm::vec3 new_hit_vec = hit_pos - target_transform->world_position;

		// if dragged
		if (glm::length2(new_hit_vec - last_hit_vec) > 1e-6f) {
			glm::vec3 axisDir = transform_axis;
			float length = glm::dot(new_hit_vec, axisDir);
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
		entt::entity arrow_x_ent = reg->create();
		auto& arrow_x_t = reg->emplace<Luxia::Components::Transform>(arrow_x_ent);

		arrow_x_t.ent_id = arrow_x_ent;
		arrow_x_t.ent_guid = Luxia::GUID(0);
		arrow_x_t.reg = reg;
		arrow_x_t.transform = &arrow_x_t;

		auto arrow_x_gizmo_part = std::make_shared<Gizmos::ScalePart>();
		arrow_x_gizmo_part->normalMat = Gizmos::GizmoResources::xMaterial;
		arrow_x_gizmo_part->transform = &arrow_x_t;
		arrow_x_gizmo_part->rot = glm::vec3(0.0f, 90.0f, 0.0f);
		arrow_x_gizmo_part->axis = Axis::x;
		;		arrow_x_gizmo_part->OnInit();

		auto& arrow_x_gizmo_behaviour = arrow_x_t.AddComponent<Gizmos::GizmoBehaviour>();
		arrow_x_gizmo_behaviour.gizmo_part = arrow_x_gizmo_part;

		// Y ARROW
		entt::entity arrow_y_ent = reg->create();
		auto& arrow_y_t = reg->emplace<Luxia::Components::Transform>(arrow_y_ent);

		arrow_y_t.ent_id = arrow_y_ent;
		arrow_y_t.ent_guid = Luxia::GUID(0);
		arrow_y_t.reg = reg;
		arrow_y_t.transform = &arrow_y_t;

		auto arrow_y_gizmo_part = std::make_shared<Gizmos::ScalePart>();
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

		auto arrow_z_gizmo_part = std::make_shared<Gizmos::ScalePart>();
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

#pragma endregion

}
