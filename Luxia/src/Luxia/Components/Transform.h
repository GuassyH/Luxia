#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "glm/common.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <entt/entt.hpp>
#include "Luxia/Components/Component.h"

namespace Luxia::Components {
	struct LUXIA_API Transform : public Component {
	public:
		// glm::vec3 vec_rotation = glm::vec3(0.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 euler_angles = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		entt::entity ent_id = entt::entity(0);
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		Transform* parent = nullptr;
		std::vector<Transform*> children;


		Transform* SetParent(Transform* new_parent) {
			// If same parent, nothing to do
			if (parent == new_parent)
				return parent;

			// Remove from old parent's children list
			if (parent) {
				auto& siblings = parent->children;
				siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
			}

			// Assign new parent
			parent = new_parent;

			// Add to new parent's children if not null
			if (new_parent) {
				new_parent->children.push_back(this);
			}

			return parent;
		}

		Transform* SetChild(Transform* new_child) {
			if (!new_child) {
				return nullptr;
			}
			new_child->SetParent(this);
			return new_child;
		}


		glm::mat4& GetMatrix() { return modelMatrix; }
		void UpdateMatrix() {
			// Build local matrix
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			glm::quat rotY = glm::angleAxis(glm::radians(euler_angles.y), glm::vec3(0, 1, 0));
			glm::quat rotX = glm::angleAxis(glm::radians(euler_angles.x), glm::vec3(1, 0, 0));
			glm::quat rotZ = glm::angleAxis(glm::radians(euler_angles.z), glm::vec3(0, 0, 1));

			glm::quat rotationQuat = rotY * rotX * rotZ;
			glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);

			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

			glm::mat4 localMatrix = translationMatrix * rotationMatrix * scaleMatrix;

			// Combine with parent matrix
			if (parent)
				modelMatrix = parent->modelMatrix * localMatrix;
			else
				modelMatrix = localMatrix;

			// NOW update children
			for (auto child : children)
				child->UpdateMatrix();
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

		entt::registry* reg = nullptr;

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			AddComponent(Args&&... args) {
			// assert(reg.valid(ent_id));
			auto& cb = reg->emplace<T>(ent_id, std::forward<Args>(args)...);
			Component* c = &cb;
			c->transform = this;
			return cb;
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			GetComponent() {
			assert(reg.valid(ent_id));
			return reg->get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T*>
			TryGetComponent() {
			return reg->try_get<T>(ent_id);
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, bool>
			HasComponent() {
			return reg->try_get<T>(ent_id) != nullptr ? true : false;
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, void>
			RemoveComponent() {
			reg->remove<T>(ent_id);
		}
	private:
	};
}


#endif