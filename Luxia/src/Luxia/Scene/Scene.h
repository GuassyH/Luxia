#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Components/Transform.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Entity.h"
#include "Luxia/Asset/Asset.h"
#include "Luxia/Core/GUID.h"

#include "entt/entt.hpp"

namespace Luxia::Assets {
	class SceneFile;
}

namespace Luxia {

	class LUXIA_API Scene : public Luxia::Assets::Asset
	{
	public:
		Scene() : reg(entt::registry()) {}
		~Scene() = default;

		virtual bool Unload() override { reg.clear(); return true; }
		void Load() { reg = entt::registry(); }

		Entity& CreateEntity(std::string name = "Entity", Luxia::GUID guid = GUID(0));
		void DeleteEntity(Luxia::GUID EntityGUID);

		// Copied and modified entt/entt.hpp view func to forward
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
		T* TryGetFromEntity(const Luxia::Entity& entity) { // Only implemented to make it more readable to me
			entt::entity = runtime_entities.find(entity.guid)->second.transform->ent_id;
			return reg.try_get<T>(entity);
		}

		template<typename T>
		T& GetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.get<T>(entity);
		}
		template<typename T>
		T& GetFromEntity(const Luxia::Entity& entity) { // Only implemented to make it more readable to me
			entt::entity = runtime_entities.find(entity.guid)->second.transform->ent_id;
			return reg.get<T>(entity);
		}


		entt::registry& GetReg() { return reg; }
		WeakPtrProxy<Luxia::Assets::SceneFile> scene_file;
		std::unordered_map<GUID, Entity> runtime_entities;
	private:
		friend class SceneSerializer;
		entt::registry reg;
	};
}

