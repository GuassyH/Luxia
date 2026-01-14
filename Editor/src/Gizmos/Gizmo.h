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

		static void Init();
	};

	/// Gizmo parts

	class GizmoPart  {
	public:
		Luxia::Components::Transform* transform = nullptr;

		GizmoPart() = default;
		virtual ~GizmoPart() = default;

		virtual bool OnClick() = 0;
		virtual glm::vec3 OnDrag() = 0;
	};


	class OrigoGizmoPart : public GizmoPart {
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

		virtual bool OnClick() override {
			return false;
		}

		virtual glm::vec3 OnDrag() override {
			return glm::vec3(0.0f);
		}
	};

	class ScaleGizmoPart : public GizmoPart {
	public:
		// which axis the arrow is responsible for
		glm::vec3 responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		virtual bool OnClick() override {
			return false;
		}

		virtual glm::vec3 OnDrag() override {
			return glm::vec3(0.0f);
		}
	};

	class RotateGizmoPart : public GizmoPart {
	public:
		// which axis the arrow is responsible for
		glm::vec3 responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

		virtual bool OnClick() override {
			return false;
		}

		virtual glm::vec3 OnDrag() override {
			return glm::vec3(0.0f);
		}
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
			entity_x->transform->euler_angles = glm::vec3(0.0f, 90.0f, 0.0f);
			entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::xMaterial);
			entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<ArrowGizmoPart> entity_y = std::make_shared<ArrowGizmoPart>();
			entity_y->transform = Create(reg);
			entity_y->transform->euler_angles = glm::vec3(-90.0f, 0.0f, 0.0f);
			entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::yMaterial);
			entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<ArrowGizmoPart> entity_z = std::make_shared<ArrowGizmoPart>();
			entity_z->transform = Create(reg);
			entity_z->transform->euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
			entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(arrow_mesh, GizmoResources::zMaterial);
			entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			std::shared_ptr<OrigoGizmoPart> entity_origo = std::make_shared<OrigoGizmoPart>();
			entity_origo->transform = Create(reg);
			entity_origo->transform->scale = glm::vec3(0.25f);
			entity_origo->transform->euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
			entity_origo->transform->AddComponent<Luxia::Components::MeshRenderer>(Luxia::ResourceManager::DefaultCube, GizmoResources::orgioMaterial);


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

		RotateGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos = path_to_gizmos.lexically_normal();

			ring_model->LoadFromPath(path_to_gizmos / "rotate/rotate.fbx");
			ring_mesh = ring_model->meshes[0];

			std::shared_ptr<RotateGizmoPart> entity_x = std::make_shared<RotateGizmoPart>();
			entity_x->transform = Create(reg);
			entity_x->transform->euler_angles = glm::vec3(0.0f, 0.0f, 90.0f);
			entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::xMaterial);
			entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<RotateGizmoPart> entity_y = std::make_shared<RotateGizmoPart>();
			entity_y->transform = Create(reg);
			entity_y->transform->euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
			entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::yMaterial);
			entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<RotateGizmoPart> entity_z = std::make_shared<RotateGizmoPart>();
			entity_z->transform = Create(reg);
			entity_z->transform->euler_angles = glm::vec3(90.0f, 0.0f, 0.0f);
			entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(ring_mesh, GizmoResources::zMaterial);
			entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			gizmo_parts.push_back(std::move(entity_x));
			gizmo_parts.push_back(std::move(entity_y));
			gizmo_parts.push_back(std::move(entity_z));
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
			entity_x->transform->euler_angles = glm::vec3(0.0f, 90.0f, 0.0f);
			entity_x->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::xMaterial);
			entity_x->responsible_axis = glm::vec3(1.0f, 0.0f, 0.0f);

			std::shared_ptr<ScaleGizmoPart> entity_y = std::make_shared<ScaleGizmoPart>();
			entity_y->transform = Create(reg);
			entity_y->transform->euler_angles = glm::vec3(-90.0f, 0.0f, 0.0f);
			entity_y->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::yMaterial);
			entity_y->responsible_axis = glm::vec3(0.0f, 1.0f, 0.0f);

			std::shared_ptr<ScaleGizmoPart> entity_z = std::make_shared<ScaleGizmoPart>();
			entity_z->transform = Create(reg);
			entity_z->transform->euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
			entity_z->transform->AddComponent<Luxia::Components::MeshRenderer>(scale_mesh, GizmoResources::zMaterial);
			entity_z->responsible_axis = glm::vec3(0.0f, 0.0f, 1.0f);

			std::shared_ptr<OrigoGizmoPart> entity_origo = std::make_shared<OrigoGizmoPart>();
			entity_origo->transform = Create(reg);
			entity_origo->transform->scale = glm::vec3(0.25f);
			entity_origo->transform->euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
			entity_origo->transform->AddComponent<Luxia::Components::MeshRenderer>(Luxia::ResourceManager::DefaultCube, GizmoResources::orgioMaterial);


			gizmo_parts.push_back(std::move(entity_x));
			gizmo_parts.push_back(std::move(entity_y));
			gizmo_parts.push_back(std::move(entity_z));
			gizmo_parts.push_back(std::move(entity_origo));
		}
	};
}