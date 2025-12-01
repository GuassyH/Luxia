#include "lxpch.h"
#include "Scene.h"

namespace Luxia {
	Components::Transform& Scene::CreateEntity() {
		entt::entity new_ent = reg.create();
		Components::Transform& t = reg.emplace<Luxia::Components::Transform>(new_ent);
		t.transform = &t;
		t.reg = &reg;
		t.ent_id = new_ent;
		return t;
	}

	void Scene::SaveToFile(const std::shared_ptr<Luxia::Assets::SceneFile> scene_file) {

	}

	void Scene::LoadFromFile(const std::shared_ptr<Luxia::Assets::SceneFile> scene_file) {
		Load();

	}
}