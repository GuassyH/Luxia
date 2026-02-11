#include "lxpch.h"
#include "Scene.h"

#include "Luxia/Core/Time.h"
#include "Luxia/Components/ComponentRegistry.h"

namespace Luxia {
	static bool name_exists(std::string name, std::unordered_map<GUID, Entity>& entities) {
		return std::any_of(entities.begin(), entities.end(),
			[&](const auto& pair) {
				return pair.second.name == name;
			});
	}

	Scene::Scene() {
		type = Luxia::AssetType::SceneType;
	}

	Entity& Scene::CreateEntity(std::string name, Luxia::GUID guid) {
		Entity ent = Entity();

		if (guid != GUID(0)) {
			ent.guid = guid;
		}
		else {
			ent.guid = GUID();
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

		int tries = 0;
		std::string new_name = name;
		// Go through each variation of the name until a new one is found
		while (name_exists(new_name, runtime_entities)) {
			tries++;
			new_name = name + " (" + std::to_string(tries) + ")";
		}

		if (tries > 0) {
			name = new_name;
		}


		ent.name = name;

		runtime_entities[ent.guid] = ent;

		return runtime_entities.find(ent.guid)->second;
	}

	void Scene::DeleteEntity(Luxia::GUID EntityGUID){
		if (runtime_entities.contains(EntityGUID)) {
			Entity& ent = runtime_entities.find(EntityGUID)->second;
			
			// VERY IMPORTANT
			// Delete all children FIRST before. This will go to the bottom of the tree, delete, and up
			for (auto child : ent.transform->children) {
				DeleteEntity(child->ent_guid);
			}
		
			// Go through each component and remove
			for (auto& comp : Luxia::componentRegistry) {
				if (ent.transform) {
					if (comp.hasFunc(ent.transform)) {
						comp.removeFunc(ent.transform);
					}
				}
			}

			if (ent.transform->HasParent())
				ent.transform->SetParent(nullptr);

			reg.remove<Luxia::Components::Transform>(ent.transform->ent_id);
			// reg.destroy(ent.transform->ent_id);

			runtime_entities.erase(EntityGUID);
		}
	}

	void Scene::Load() {
		reg = entt::registry();
		physicsWorld = Physics::PhysicsSystem::CreateWorld(Physics::PhysicsWorldDesc()); // Create with standard desc
	}

	bool Scene::Unload() {
		std::vector<GUID> toDelete;
		toDelete.reserve(runtime_entities.size());

		for (auto& [guid, entity] : runtime_entities)
			toDelete.push_back(guid);

		for (auto& guid : toDelete)
			DeleteEntity(guid);

		runtime_entities.clear();
		reg.clear();
		toDelete.clear();

		Physics::PhysicsSystem::DeleteWorld(physicsWorld.get()); // Unneccessary
		
		physicsWorld.release();
		physicsWorld.reset();

		return true;
	}

	void Scene::Start() {
		isStarted = true;

		auto& body_interface = physicsWorld->jphSystem.GetBodyInterface();
		for (auto entity : GetEntitiesWith<Luxia::Components::RigidBody>()) {
			auto& rb = GetFromEntity<Luxia::Components::RigidBody>(entity);

			rb.InitBody(body_interface);
		}
	}

	void Scene::Update() {
		// Update physics
		auto rigid_view = GetEntitiesWith<Luxia::Components::RigidBody>();
		for (auto entity : rigid_view) {
			auto& rb = GetFromEntity<Luxia::Components::RigidBody>(entity);
			rb.body->SetMotionType(rb.motionType);
		}
	
		physicsWorld->step(Luxia::Core::Time::get().deltaTime);
		
		auto& body_interface = physicsWorld->jphSystem.GetBodyInterface();

		// Go through each entity, get their position and rotation
		for (auto entity : rigid_view) {
			auto& rb = GetFromEntity<Luxia::Components::RigidBody>(entity);
			rb.CalculatePosition(Physics::ToGLM(body_interface.GetPosition(rb.body->GetID())));
			rb.CalculateRotation(Physics::ToGLM(body_interface.GetRotation(rb.body->GetID())));
		}
	}

	void Scene::End() {
		isStarted = false;

		auto& body_interface = physicsWorld->jphSystem.GetBodyInterface();
		for (auto entity : GetEntitiesWith<Luxia::Components::RigidBody>()) {
			auto& rb = GetFromEntity<Luxia::Components::RigidBody>(entity);

			rb.UnloadBody(body_interface);
		}
	}
}