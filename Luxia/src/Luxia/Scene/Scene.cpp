#include "lxpch.h"
#include "Scene.h"

namespace Luxia {
	Entity& Scene::CreateEntity(std::string name, Luxia::GUID guid) {
		Entity ent = Entity();

		if (guid != GUID(0)) {
			ent.guid = guid;
		}

		entt::entity new_entt = reg.create();
		reg.emplace<Luxia::Components::Transform>(new_entt);
		Luxia::Components::Transform* t = reg.try_get<Luxia::Components::Transform>(new_entt);
		t->transform = t;
		t->reg = &reg;
		t->ent_id = new_entt;
		t->ent_guid = ent.guid;

		ent.transform = t;
		ent.transform->ent_guid = ent.guid;
		ent.name = name;

		runtime_entities[ent.guid] = ent;

		return runtime_entities.find(ent.guid)->second;
	}
}