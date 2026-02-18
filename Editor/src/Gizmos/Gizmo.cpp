#include "Gizmo.h"

namespace Editor::Gizmos {

	std::shared_ptr<Luxia::IMaterial> GizmoResources::xMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::yMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::zMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::orgioMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::hoverMaterial = nullptr;

	std::shared_ptr<Luxia::Mesh> GizmoResources::arrowMesh = nullptr;

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
	
	
	}


	GizmoCollection ArrowCollection(entt::registry* reg) {
		Gizmos::GizmoCollection collection = Gizmos::GizmoCollection();
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

		auto arrow_y_gizmo_part = std::make_shared<Gizmos::ArrowPart>();
		arrow_y_gizmo_part->normalMat = Gizmos::GizmoResources::yMaterial;
		arrow_y_gizmo_part->transform = &arrow_y_t;
		arrow_y_gizmo_part->rot = glm::vec3(-90.0f, 0.0f, 0.0f);
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
		collection.behaviours.push_back(arrow_x_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		collection.behaviours.push_back(arrow_y_t.TryGetComponent<Gizmos::GizmoBehaviour>());
		collection.behaviours.push_back(arrow_z_t.TryGetComponent<Gizmos::GizmoBehaviour>());

		return collection;
	}
}
