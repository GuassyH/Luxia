#include "lxpch.h"
#include "Scene.h"

namespace Luxia {
	Components::Transform& Scene::CreateEntity() {
		entt::entity new_ent = reg.create();
		Components::Transform& t = reg.emplace<Luxia::Components::Transform>(new_ent);
		t.transform = std::make_shared<Luxia::Components::Transform>(t);
		t.ent_id = new_ent;
		return t;
	}
}