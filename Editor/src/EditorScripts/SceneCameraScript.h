#pragma once

#include "Luxia.h"
#include "glm/glm.hpp"

namespace Editor::Scripts {

	class SceneCameraScript : public Luxia::Components::Component {
	public:
		float speed = 1.0f;
		float sensitivity = 0.002f;

		glm::vec3 moveDir;
		float horizontal = 0.0f;
		float vertical = 0.0f;

		void Move(Luxia::Components::Transform* focused_t) {
			glm::vec3 forward = glm::normalize(transform->GetRotVec());
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 up = glm::normalize(glm::cross(right, forward));

			horizontal = 0.0f;
			vertical = 0.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_D))
				horizontal += 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_A))
				horizontal -= 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_W))
				vertical += 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_S))
				vertical -= 1.0f;

			moveDir = right * horizontal + forward * vertical;
			
			transform->position += moveDir * speed * 0.01f; // Should multiply with delta time


			if (Luxia::Input::IsKeyJustPressed(LX_KEY_F)) {
				if (focused_t) {
					transform->position = focused_t->world_position + (transform->GetRotVec() * -10.0f);
				}
			}
		}

		double last_mouseX;
		double last_mouseY;

		void Look() {
			if (!transform) { return; }

			glm::vec2 mousePos = Luxia::Input::GetMousePosition();

			double mouseX = mousePos.x;
			double mouseY = mousePos.y;

			// double centerX = glm::roundEven(w_pos.x + (w_size.x / 2.0f));
			// double centerY = glm::roundEven(w_pos.y + (w_size.y / 2.0f));

			// Should be clamped to window size

			float deltaX = (float)(mouseX - last_mouseX);
			float deltaY = (float)(mouseY - last_mouseY);

			float rotX = -deltaY * sensitivity * 100.0f;
			float rotY = deltaX * sensitivity * 100.0f;

			// Apply rotations to Euler angles
			transform->euler_angles.x -= rotX; // pitch
			transform->euler_angles.y += rotY; // yaw

			// Clamp pitch to avoid flipping
			if (transform->euler_angles.x > 89.0f) transform->euler_angles.x = 89.0f;
			if (transform->euler_angles.x < -89.0f) transform->euler_angles.x = -89.0f;

			if (Luxia::Input::GetScrollOffset().y != 0) {
				speed = glm::fclamp(speed * (Luxia::Input::GetScrollOffset().y > 0 ? 2.0f : 0.5f), 0.5f, 256.0f);
			}

			last_mouseX = mouseX;
			last_mouseY = mouseY;
		}
	};

}