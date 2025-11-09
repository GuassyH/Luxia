#pragma once

#include "Luxia/Core/Core.h"
#include "entt/entt.hpp"
#include "Luxia/Components/Transform.h"

namespace Luxia {

	class LUXIA_API Scene
	{
	public:
		Scene() : reg(entt::registry()) {}
		~Scene() = default;

		void Cleanup() { reg.clear(); }
		void Load() { reg = entt::registry(); }

		Components::Transform& CreateEntity();
		
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
		T& GetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.get<T>(entity);
		}

		entt::registry& GetReg() { return reg; }
	private:
		entt::registry reg;
		// guid
	};
}

