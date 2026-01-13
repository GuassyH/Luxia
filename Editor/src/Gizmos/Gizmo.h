#pragma once

#include <unordered_map>
#include "Luxia/Core/GUID.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Entity.h"
#include "Luxia/Components/ComponentRegistry.h"


namespace Editor::Gizmos {

	class Gizmo {
	public:
		Gizmo() = default;
		virtual ~Gizmo() = default;

		std::vector<entt::entity> gizmo_entities;

		virtual bool OnClick() = 0;
		virtual bool OnDrag() = 0;
	};

	static Luxia::Components::Transform& CreateGizmoMesh(entt::registry& reg, const char* name, std::shared_ptr<Luxia::Mesh> mesh, std::shared_ptr<Luxia::IMaterial> material) {
		entt::entity entity = reg.create();
		auto& transform = reg.emplace<Luxia::Components::Transform>(entity);
		transform.ent_id = entity;
		transform.reg = &reg;
		reg.emplace<Luxia::Components::MeshRenderer>(entity, mesh, material);
		return transform;
	}


	class TranslateGizmo : public Gizmo {
	public:
		std::shared_ptr<Luxia::IModel> arrow_model = Luxia::Platform::Assets::CreateModel();
		std::shared_ptr<Luxia::Mesh> arrow_mesh = nullptr;

		std::shared_ptr<Luxia::IMaterial> xMaterial = Luxia::Platform::Assets::CreateMaterial();
		std::shared_ptr<Luxia::IMaterial> yMaterial = Luxia::Platform::Assets::CreateMaterial();
		std::shared_ptr<Luxia::IMaterial> zMaterial = Luxia::Platform::Assets::CreateMaterial();

		TranslateGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos = path_to_gizmos.lexically_normal();

			xMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
			xMaterial->color = glm::vec4(255.0f / 255.0f, 112.0f / 255.0f, 122.0f / 255.0f, 1.0f);

			yMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
			yMaterial->color = glm::vec4(142.0f / 255.0f, 255.0f / 255.0f, 142.0f / 255.0f, 1.0f);

			zMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
			zMaterial->color = glm::vec4(45.0f / 255.0f, 230.0f / 255.0f, 255.0f / 255.0f, 1.0f);

			arrow_model->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
			arrow_mesh = arrow_model->meshes[0];

			auto& entity_x = CreateGizmoMesh(reg, "ArrowX", arrow_mesh, xMaterial);
			entity_x.euler_angles = glm::vec3(0.0f, 90.0f, 0.0f);
			
			auto& entity_y = CreateGizmoMesh(reg, "ArrowY", arrow_mesh, yMaterial);
			entity_y.euler_angles = glm::vec3(-90.0f, 0.0f, 0.0f);

			auto& entity_z = CreateGizmoMesh(reg, "ArrowZ", arrow_mesh, zMaterial);
			entity_z.euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);

			gizmo_entities.push_back(entity_x.ent_id);
			gizmo_entities.push_back(entity_y.ent_id);
			gizmo_entities.push_back(entity_z.ent_id);

			LX_INFO("Gizmos: Initialized Translation Gizmo [{}]", gizmo_entities.size());
		}

		virtual bool OnClick() override {
			return true;
		}

		virtual bool OnDrag() override {
			return true;
		}
	};

}