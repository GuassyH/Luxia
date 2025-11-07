#pragma once

#include "Luxia/Core/Core.h"
#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "Luxia/Components/Component.h"

namespace Luxia::Components {
	struct LUXIA_API Transform : public Component {
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		entt::entity ent_id;

		Transform()
			: position(glm::vec3(0.0f)), 
			rotation(glm::vec3(0.0f)),
			scale(glm::vec3(1.0f)) {}
	


		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T*>
			AddComponent(entt::registry& reg, Args&&... args) {
			reg.emplace<T>(ent_id, std::forward<Args>(args)...);
			return reg.try_get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T*>
			GetComponent(entt::registry& reg){
			return reg.try_get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, bool>
			HasComponent(entt::registry& reg) {
			return reg.try_get<T>(ent_id) != nullptr ? true : false;
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, void>
			RemoveComponent(entt::registry& reg) {
			reg.remove<T>(ent_id);
		}
	};
}