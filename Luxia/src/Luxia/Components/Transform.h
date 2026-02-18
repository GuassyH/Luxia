#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <entt/entt.hpp>
#include "Luxia/Components/Component.h"

#include "Luxia/Events/EventHandler.h"

namespace Luxia::Components {
	struct LUXIA_API Transform : public Component {
	public:
		// Local variables (set)
		glm::vec3 local_position = glm::vec3(0.0f); // (get & set)
		glm::vec3 local_scale = glm::vec3(1.0f); // (get & set)
		glm::quat local_rotation = glm::identity<glm::quat>(); // (get & set)

		// Directions
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		
		// Global variables (get)
		glm::vec3 world_position = glm::vec3(0.0f); // (get only)
		glm::vec3 world_scale = glm::vec3(0.0f);  // (get only)
		glm::quat world_rotation = glm::identity<glm::quat>(); // (get only)

		// Transform
		entt::entity ent_id = entt::entity(0);
		std::vector<Transform*> children;
		Transform* parent = nullptr;

		bool HasParent() const {
			return parent != nullptr;
		}

		bool IsDescendant(Transform* parent, Transform* child) {
			for (auto* p = parent; p; p = p->parent)
				if (p == child)
					return true;
			return false;
		}

		// THE RETRANSFORM IS WRONG
		Transform* SetParent(Transform* new_parent, bool change_translate = true) {
			// If same parent, nothing to do
			if (parent == new_parent)
				return parent;
			if (IsDescendant(new_parent, this))
				return parent;
			

			// Remove from old parent's children list
			if (parent) {
				auto& siblings = parent->children;
				siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
			}

			// Recalculate position and stuff
			if (parent && change_translate) {
				local_scale *= parent->world_scale;
				local_position += parent->world_position;
				local_rotation = parent->world_rotation * local_rotation;
			}
			
			// Assign new parent
			parent = new_parent;

			// Add to new parent's children if not null (/ setting parent to world)
			if (parent) {
				parent->children.push_back(this);

				if (change_translate) {
					// Recalculate position and stuff
					local_scale.x /= parent->world_scale.x != 0 ? parent->world_scale.x : 1.0f;
					local_scale.y /= parent->world_scale.y != 0 ? parent->world_scale.y : 1.0f;
					local_scale.z /= parent->world_scale.z != 0 ? parent->world_scale.z : 1.0f;

					local_position -= parent->world_position;
					local_rotation = glm::inverse(parent->world_rotation) * local_rotation;
				}
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
		glm::mat3 GetRotationMatrix() { return glm::mat3(glm::transpose(glm::inverse(modelMatrix))); }
		
		void UpdateMatrix() {
			// Combine with parent matrix
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), local_scale);
			glm::mat4 rotationMatrix = glm::mat4_cast(local_rotation);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), local_position);
			glm::mat4 localModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
			
			if (parent) {
				world_position = parent->local_position + local_position;
				world_scale = local_scale * parent->local_scale;
				world_rotation = glm::normalize(parent->local_rotation * local_rotation);
				modelMatrix = parent->GetMatrix() * localModelMatrix;
 			}
			else {
				world_position = local_position;
				world_scale = local_scale;
				world_rotation = local_rotation;
				modelMatrix = localModelMatrix;
			}

			// forward.y = -forward.y;
			forward = glm::normalize(world_rotation * glm::vec3(0, 0, -1));
			up = glm::normalize(world_rotation * glm::vec3(0, 1, 0));
			right = glm::normalize(world_rotation * glm::vec3(1, 0, 0));

			// NOW update children
			for (auto child : children)
				child->UpdateMatrix();
		}

		// Get rotation in Euler angles (degrees)
		glm::vec3 GetEulerAngles(bool local = true) const {
			if(local)
				return glm::degrees(glm::eulerAngles(local_rotation));
			else
				return glm::degrees(glm::eulerAngles(world_rotation));

		}

		// Set rotation from Euler angles (degrees)
		void SetEulerAngles(const glm::vec3& euler_angles_deg) {
			glm::vec3 euler_rad = glm::radians(euler_angles_deg);
			local_rotation = glm::normalize(glm::quat(euler_rad));
		}

		// Add rotation in Euler angles (degrees), applied in local space
		void AddEulerAngles(const glm::vec3& euler_angles_deg, bool local = true) {
			glm::quat delta = glm::normalize(glm::quat(glm::radians(euler_angles_deg)));

			if (local)
				local_rotation = glm::normalize(local_rotation * delta); // local space
			else
				local_rotation = glm::normalize(delta * local_rotation); // world space
		}

		void AddRotationAroundAxis(const glm::vec3& axis, float angle_deg, bool local = true) {
			glm::quat delta = glm::angleAxis(glm::radians(angle_deg), glm::normalize(axis));

			if (local)
				local_rotation = glm::normalize(local_rotation * delta); // local space
			else
				local_rotation = glm::normalize(delta * local_rotation); // world space
		}
		Transform() = default;


		entt::registry* reg = nullptr;

		// Inline header?
#pragma region GetAdd
		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			AddComponent(Args&&... args) {
			// assert(reg.valid(ent_id));
			if (reg->try_get<T>(ent_id)) {
				LX_CORE_ERROR("Tried to add already added component to ent - {}", (uint64_t)ent_guid);
				return reg->get<T>(ent_id);
			}

			auto& cb = reg->emplace<T>(ent_id, std::forward<Args>(args)...);
			Component* c = &cb;
			c->transform = this;
			c->ent_guid = this->ent_guid;
			return cb;
		}

		template <typename T, typename... Args>
		std::enable_if_t<std::is_base_of_v<Luxia::Components::Component, T>, T&>
			GetComponent() {
			// assert(reg.valid(ent_id));
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
#pragma endregion

		virtual void OnInspectorDraw() override {
			ImGui::DragFloat3("Position", &local_position.x, 0.1f);

			glm::vec3 euler = GetEulerAngles();
			if (ImGui::DragFloat3("Rotation", &euler.x, 0.1f)) {
				glm::vec3 delta = euler - GetEulerAngles();
				AddEulerAngles(delta, false); // apply as incremental rotation
			}
			ImGui::DragFloat3("Scale", &local_scale.x, 0.1f);
		}

	private:
		glm::mat4 modelMatrix = glm::mat4(1.0f);
	};
}


#endif