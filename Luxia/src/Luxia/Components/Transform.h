#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "Luxia/Core/Core.h"
#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "Luxia/Components/Component.h"

namespace Luxia::Components {
	struct LUXIA_API Transform : public Component {
	private:
		glm::mat4 modelMatrix = glm::mat4(1.0f);
	public:
		// glm::vec3 vec_rotation = glm::vec3(0.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		entt::entity ent_id;

		glm::mat4& GetMatrix() { return modelMatrix; }
		void UpdateMatrix(){
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			// Construct rotation quat (Y > X > Z)
			glm::quat rotY = glm::angleAxis(glm::radians(euler_angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat rotX = glm::angleAxis(glm::radians(euler_angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::quat rotZ = glm::angleAxis(glm::radians(euler_angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::quat rotationQuat = rotY * rotX * rotZ; // Apply Z rotation first, then X, then Y

			glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

			// Combine to get model matrix: translate * rotate * scale
			modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		}
		glm::vec3 GetRotVec() const {
			float pitch = glm::radians(euler_angles.x);
			float yaw = glm::radians(euler_angles.y);

			glm::vec3 direction = glm::vec3(0.0f);
			direction.z = -(cos(pitch) * cos(yaw));
			direction.y = -sin(pitch);
			direction.x = (cos(pitch) * sin(yaw));

			return direction;
		}
		glm::vec3 VecToDeg(const glm::vec3& rad) {
			return glm::degrees(rad);
		}

		Transform() = default;

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			AddComponent(entt::registry& reg, Args&&... args) {
			auto cb = reg.emplace<T>(ent_id, std::forward<Args>(args)...);
			Component* c = &cb;
			c->transform = this->transform;
			return reg.get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			GetComponent(entt::registry& reg) {
			return reg.get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T*>
			TryGetComponent(entt::registry& reg) {
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


#endif