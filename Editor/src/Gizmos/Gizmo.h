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


	class TranslateGizmo : public Gizmo {
	public:
		TranslateGizmo(entt::registry& reg, std::filesystem::path path_to_gizmos) {
			path_to_gizmos.lexically_normal();

			std::shared_ptr<Luxia::IModel> arrow_model = Luxia::Platform::Assets::CreateModel();
			arrow_model->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
			std::shared_ptr<Luxia::Mesh> arrow_mesh = arrow_model->meshes[0];

			entt::entity x = reg.create();
			reg.emplace<Luxia::Components::Transform>(x);
			reg.emplace<Luxia::Components::MeshRenderer>(x, arrow_mesh, nullptr);

			entt::entity y = reg.create();
			reg.emplace<Luxia::Components::Transform>(y);
			reg.emplace<Luxia::Components::MeshRenderer>(y, arrow_mesh, nullptr);

			entt::entity z = reg.create();
			reg.emplace<Luxia::Components::Transform>(z);
			reg.emplace<Luxia::Components::MeshRenderer>(z, arrow_mesh, nullptr);

			gizmo_entities.push_back(x);
			gizmo_entities.push_back(y);
			gizmo_entities.push_back(z);

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