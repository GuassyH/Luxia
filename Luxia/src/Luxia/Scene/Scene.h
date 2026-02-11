#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Components/Transform.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Entity.h"
#include "Luxia/Asset/Asset.h"
#include "Luxia/Core/GUID.h"

#include "entt/entt.hpp"
#include "Luxia/Components/Camera.h"
#include "Luxia/Components/Light.h"
#include "Luxia/Physics/PhysicsWorld.h"
#include "Luxia/Physics/PhysicsSystem.h"

namespace Luxia::Assets {
	class SceneFile;
}

namespace Luxia {

	class LUXIA_API Scene : public Luxia::Assets::Asset
	{
	public:
		Scene();
		~Scene() = default;

		void Load();
		virtual bool Unload() override;
		// Only Creates the reg

		void Start();
		void Update();
		void End();

		bool isStarted = false;

		Entity& CreateEntity(std::string name = "Entity", Luxia::GUID guid = GUID(0));
		void DeleteEntity(Luxia::GUID EntityGUID);

		// Copied and modified entt/entt.hpp view func to forward
#pragma region Templates
		template<typename Type, typename... Other, typename... Exclude>
		entt::basic_view<entt::get_t<entt::storage_for_t<Type>, entt::storage_for_t<Other>...>, entt::exclude_t<entt::storage_for_t<Exclude>...>>
			GetEntitiesWith(entt::exclude_t<Exclude...> = entt::exclude_t{}) {
			return reg.view<Type>();
		}

		template<typename T>
		T* TryGetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.try_get<T>(entity);
		}
		template<typename T>
		T* TryGetFromEntity(const Luxia::Entity& entity) {
			auto it = runtime_entities.find(entity.guid);
			if (it == runtime_entities.end() || !it->second.transform) return nullptr;
			entt::entity ent = it->second.transform->ent_id;
			return reg.try_get<T>(ent);
		}

		template<typename T>
		T& GetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.get<T>(entity);
		}

		template<typename T>
		T& GetFromEntity(const Luxia::Entity& entity) {
			auto it = runtime_entities.find(entity.guid);
			LX_CORE_ASSERT_MSG(it != runtime_entities.end() && it->second.transform, "Entity not found");
			entt::entity ent = it->second.transform->ent_id;
			return reg.get<T>(ent);
		}
#pragma endregion

		// Mains
		Luxia::Components::Light* GetMainLight() {
			auto light_view = GetEntitiesWith<Luxia::Components::Light>();
			for (const auto& light_ent : light_view) {
				auto& l = reg.get<Luxia::Components::Light>(light_ent);
				if (l.lightType == Luxia::LightType::Directional)
					return &l;
				else continue;
			}
			return nullptr;
		}
		Luxia::Components::Camera* GetMainCamera() {
			auto cam_view = GetEntitiesWith<Luxia::Components::Camera>();
			for (const auto& cam_ent : cam_view) {
				auto& c = reg.get<Luxia::Components::Camera>(cam_ent);
				if (c.main) 
					return &c;
				else continue;
			}
			return nullptr;
		}

		std::unique_ptr<Physics::PhysicsWorld> physicsWorld = nullptr;
		entt::registry& GetReg() { return reg; }
		WeakPtrProxy<Luxia::Assets::SceneFile> scene_file;
		std::unordered_map<GUID, Entity> runtime_entities;
	private:
		friend class SceneSerializer;
		entt::registry reg;
	};
}

