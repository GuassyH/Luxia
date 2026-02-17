#pragma once

#include <unordered_map>
#include "Luxia/Core/GUID.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Entity.h"
#include "Luxia/Components/ComponentRegistry.h"
#include "Luxia/Managers/ResourceManager.h"


namespace Editor::Gizmos {

	/// Resources

	class GizmoResources {
	public:
		static std::shared_ptr<Luxia::IMaterial> xMaterial;
		static std::shared_ptr<Luxia::IMaterial> yMaterial;
		static std::shared_ptr<Luxia::IMaterial> zMaterial;
		static std::shared_ptr<Luxia::IMaterial> orgioMaterial;
		static std::shared_ptr<Luxia::IMaterial> hoverMaterial;

		static void Init();
	};

	/// Gizmo parts

	class GizmoPart  {
	public:
		Luxia::Components::Transform* transform = nullptr;

		std::shared_ptr<Luxia::IMaterial> material = nullptr;

		GizmoPart() = default;
		virtual ~GizmoPart() = default;

		virtual bool OnClick() = 0;
		virtual glm::vec3 OnDrag() = 0;
	};


	class BasicGizmoPart : public GizmoPart {
	public:
		// does nothing
		virtual bool OnClick() override {
			return false;
		}
		// does nothing
		virtual glm::vec3 OnDrag() override {
			return glm::vec3(0.0f);
		}
	};

	class ArrowGizmoPart : public GizmoPart {
	public:
		// which axis the arrow is responsible for
		glm::vec3 responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		virtual bool OnClick() override;
		virtual glm::vec3 OnDrag() override;
	};

	class ScaleGizmoPart : public GizmoPart {
	public:
		// which axis the arrow is responsible for
		glm::vec3 responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		virtual bool OnClick() override;
		virtual glm::vec3 OnDrag() override;
	};

	class RotateGizmoPart : public GizmoPart {
	public:
		// which axis the arrow is responsible for
		glm::vec3 responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		virtual bool OnClick() override;
		virtual glm::vec3 OnDrag() override;
	};

	/// Gizmos

	class Gizmo {
	public:
		Gizmo() = default;
		virtual ~Gizmo() = default;

		std::vector<std::shared_ptr<GizmoPart>> gizmo_parts;
	};

	static Luxia::Components::Transform* Create(entt::registry& reg) {
		entt::entity entity = reg.create();
		auto& transform = reg.emplace<Luxia::Components::Transform>(entity);
		transform.ent_id = entity;
		transform.reg = &reg;

		return &transform;
	}

	class GridGizmo : public Gizmo {
	public:
		std::shared_ptr<Luxia::IShader> gridShader = nullptr;
		std::shared_ptr<Luxia::IMaterial> gridMaterial = nullptr;

		GridGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			gridShader = Luxia::Platform::Assets::CreateShader((path_to_gizmos / "grid.frag").string().c_str(), (path_to_gizmos / "grid.vert").string().c_str());
			gridMaterial = Luxia::Platform::Assets::CreateMaterial(gridShader);

			std::shared_ptr<BasicGizmoPart> grid_plane = std::make_shared<BasicGizmoPart>();
			grid_plane->transform = Create(reg);
			grid_plane->transform->AddComponent<Luxia::Components::MeshRenderer>(Luxia::ResourceManager::DefaultPlane, gridMaterial);
			grid_plane->transform->local_scale = glm::vec3(1000.0f, 1.0f, 1000.0f);

			gizmo_parts.push_back(std::move(grid_plane));
		}
	};

	class TranslateGizmo : public Gizmo {
	public:
		std::shared_ptr<Luxia::IModel> arrow_model = Luxia::Platform::Assets::CreateModel();
		std::shared_ptr<Luxia::Mesh> arrow_mesh = nullptr;

		TranslateGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos = path_to_gizmos.lexically_normal();

			arrow_model->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
			arrow_mesh = arrow_model->meshes[0];

			std::shared_ptr<ArrowGizmoPart> entity_x = std::make_shared<ArrowGizmoPart>();
			entity_x->transform = Create(reg);
			entity_x->transform->AddEulerAngles(glm::vec3(0.0f, 90.0f, 0.0f));
			entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::xMaterial);
			entity_x->material = GizmoResources::xMaterial;
			entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<ArrowGizmoPart> entity_y = std::make_shared<ArrowGizmoPart>();
			entity_y->transform = Create(reg);
			entity_y->transform->AddEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
			entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::yMaterial);
			entity_y->material = GizmoResources::yMaterial;
			entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<ArrowGizmoPart> entity_z = std::make_shared<ArrowGizmoPart>();
			entity_z->transform = Create(reg);
			entity_z->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::zMaterial);
			entity_z->material = GizmoResources::zMaterial;
			entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			std::shared_ptr<BasicGizmoPart> entity_origo = std::make_shared<BasicGizmoPart>();
			entity_origo->transform = Create(reg);
			entity_origo->transform->local_scale = glm::vec3(0.25f);
			entity_origo->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			entity_origo->transform->AddComponent<Luxia::Components::MeshRenderer>(Luxia::ResourceManager::DefaultCube, GizmoResources::orgioMaterial);
			entity_origo->material = GizmoResources::orgioMaterial;

			entity_x->transform->AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
			auto& x_col = entity_x->transform->AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
			x_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
			x_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
			x_col.InitCollider();

			entity_y->transform->AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
			auto& y_col = entity_y->transform->AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
			y_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
			y_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
			y_col.InitCollider();

			entity_z->transform->AddComponent<Luxia::Components::RigidBody>().motionType = JPH::EMotionType::Kinematic;
			auto& z_col = entity_z->transform->AddComponent<Luxia::Components::Collider>(Luxia::Collider::Box);
			z_col.scale = glm::vec3(0.3f, 0.3f, 1.0f);
			z_col.offset = glm::vec3(0.0f, 0.0f, 1.0f);
			z_col.InitCollider();


			gizmo_parts.push_back(std::move(entity_x));
			gizmo_parts.push_back(std::move(entity_y));
			gizmo_parts.push_back(std::move(entity_z));
			gizmo_parts.push_back(std::move(entity_origo));
		}
	};

	class RotateGizmo : public Gizmo {
	public:
		std::shared_ptr<Luxia::IModel> ring_model = Luxia::Platform::Assets::CreateModel();
		std::shared_ptr<Luxia::Mesh> ring_mesh = nullptr;

		std::shared_ptr<Luxia::IModel> arrow_model = Luxia::Platform::Assets::CreateModel();
		std::shared_ptr<Luxia::Mesh> arrow_mesh = nullptr;

		RotateGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos = path_to_gizmos.lexically_normal();

			ring_model->LoadFromPath(path_to_gizmos / "rotate/rotate.fbx");
			ring_mesh = ring_model->meshes[0];

			// arrow_model->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
			arrow_mesh = ring_model->meshes[1];

			std::shared_ptr<RotateGizmoPart> rot_entity_x = std::make_shared<RotateGizmoPart>();
			rot_entity_x->transform = Create(reg);
			rot_entity_x->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 90.0f));
			rot_entity_x->transform->local_scale = glm::vec3(0.9f);
			rot_entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::xMaterial);
			rot_entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<RotateGizmoPart> rot_entity_y = std::make_shared<RotateGizmoPart>();
			rot_entity_y->transform = Create(reg);
			rot_entity_y->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			rot_entity_y->transform->local_scale = glm::vec3(0.9f);
			rot_entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::yMaterial);
			rot_entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<RotateGizmoPart> rot_entity_z = std::make_shared<RotateGizmoPart>();
			rot_entity_z->transform = Create(reg);
			rot_entity_z->transform->AddEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
			rot_entity_z->transform->local_scale = glm::vec3(0.9f);
			rot_entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::zMaterial);
			rot_entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			std::shared_ptr<BasicGizmoPart> origo_entity_x = std::make_shared<BasicGizmoPart>();
			origo_entity_x->transform = Create(reg);
			origo_entity_x->transform->AddEulerAngles(glm::vec3(0.0f, 90.0f, 0.0f));
			origo_entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::orgioMaterial);

			std::shared_ptr<BasicGizmoPart> origo_entity_y = std::make_shared<BasicGizmoPart>();
			origo_entity_y->transform = Create(reg);
			origo_entity_y->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			origo_entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::orgioMaterial);

			std::shared_ptr<BasicGizmoPart> origo_entity_z = std::make_shared<BasicGizmoPart>();
			origo_entity_z->transform = Create(reg);
			origo_entity_z->transform->AddEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
			origo_entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::orgioMaterial);

			gizmo_parts.push_back(std::move(rot_entity_x));
			gizmo_parts.push_back(std::move(rot_entity_y));
			gizmo_parts.push_back(std::move(rot_entity_z));

			gizmo_parts.push_back(std::move(origo_entity_x));
			gizmo_parts.push_back(std::move(origo_entity_y));
			gizmo_parts.push_back(std::move(origo_entity_z));
		}
	};


	class ScaleGizmo : public Gizmo {
	public:
		std::shared_ptr<Luxia::IModel> scale_model = Luxia::Platform::Assets::CreateModel();
		std::shared_ptr<Luxia::Mesh> scale_mesh = nullptr;

		ScaleGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos = path_to_gizmos.lexically_normal();

			scale_model->LoadFromPath(path_to_gizmos / "scale/scale.fbx");
			scale_mesh = scale_model->meshes[0];

			std::shared_ptr<ScaleGizmoPart> entity_x = std::make_shared<ScaleGizmoPart>();
			entity_x->transform = Create(reg);
			entity_x->transform->AddEulerAngles(glm::vec3(0.0f, 90.0f, 0.0f));
			entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::xMaterial);
			entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<ScaleGizmoPart> entity_y = std::make_shared<ScaleGizmoPart>();
			entity_y->transform = Create(reg);
			entity_y->transform->AddEulerAngles(glm::vec3(-90.0f, 0.0f, 0.0f));
			entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::yMaterial);
			entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<ScaleGizmoPart> entity_z = std::make_shared<ScaleGizmoPart>();
			entity_z->transform = Create(reg);
			entity_z->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::zMaterial);
			entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			std::shared_ptr<BasicGizmoPart> entity_origo = std::make_shared<BasicGizmoPart>();
			entity_origo->transform = Create(reg);
			entity_origo->transform->local_scale = glm::vec3(0.25f);
			entity_origo->transform->AddEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
			entity_origo->transform->AddComponent<Luxia::Components::MeshRenderer>(Luxia::ResourceManager::DefaultCube, GizmoResources::orgioMaterial);


			gizmo_parts.push_back(std::move(entity_x));
			gizmo_parts.push_back(std::move(entity_y));
			gizmo_parts.push_back(std::move(entity_z));
			gizmo_parts.push_back(std::move(entity_origo));
		}
	};
}