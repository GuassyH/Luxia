#pragma once

#include "Luxia.h"
#include "glm/glm.hpp"

namespace Editor::Scripts {

	class SceneCameraScript : public Luxia::Components::Component {
	public:
		float speed = 10.0f;
		float sensitivity = 0.002f;

		glm::vec3 moveDir = glm::vec3(0.0f);
		float horizontal = 0.0f;
		float vertical = 0.0f;
		float skywards = 0.0f;

		enum MovementPresets {
			Me = 0,
			Xeffi = 1,
			Aaron = 2
		};

		MovementPresets movement_preset = MovementPresets::Me;

		void Move() {

			horizontal = 0.0f;
			vertical = 0.0f;
			skywards = 0.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_D))
				horizontal += 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_A))
				horizontal -= 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_W))
				vertical += 1.0f;

			if (Luxia::Input::IsKeyPressed(LX_KEY_S))
				vertical -= 1.0f;

			int UpKey = 0;
			int DownKey = 1;

			switch (movement_preset) {
			case Editor::Scripts::SceneCameraScript::Me:{
				UpKey = LX_KEY_E;
				DownKey = LX_KEY_Q;
				break;
			}
			case Editor::Scripts::SceneCameraScript::Xeffi: {
				UpKey = LX_KEY_SPACE;
				DownKey = LX_KEY_C;
				break;
			}
			case Editor::Scripts::SceneCameraScript::Aaron: {
				UpKey = LX_KEY_F;
				DownKey = LX_KEY_C;
				break;
			}
			default:
				UpKey = LX_KEY_E;
				DownKey = LX_KEY_Q;
				break;
			}

			if (Luxia::Input::IsKeyPressed(UpKey))
				skywards += 1.0f;

			if (Luxia::Input::IsKeyPressed(DownKey))
				skywards -= 1.0f;

			moveDir = transform->right * horizontal + transform->forward * vertical;
			moveDir = glm::length(moveDir) != 0 ? glm::normalize(moveDir) : glm::vec3(0);
			moveDir.y += skywards;

			transform->local_position += moveDir * speed * Luxia::Core::Time::get().deltaTime;

			// LX_INFO("Up = {}x {}y {}z", transform->up.x, transform->up.y, transform->up.z);
		}

		double last_mouseX = 0.0;
		double last_mouseY = 0.0;

		void Look() {
			if (!transform) { return; }

			glm::vec2 mousePos = Luxia::Input::GetMousePosition();

			double mouseX = mousePos.x;
			double mouseY = mousePos.y;

			// double centerX = glm::roundEven(w_pos.x + (w_size.x / 2.0f));
			// double centerY = glm::roundEven(w_pos.y + (w_size.y / 2.0f));

			// Should be clamped to window size

			float deltaX = static_cast<float>(mouseX - last_mouseX);
			float deltaY = static_cast<float>(mouseY - last_mouseY);

			float rotX = -deltaY * sensitivity * 100.0f;
			float rotY = deltaX * sensitivity * 100.0f;

			// Apply rotations to Euler angles
			transform->local_euler_angles.x += rotX; // pitch
			transform->local_euler_angles.y -= rotY; // yaw

			// Clamp pitch to avoid flipping
			if (transform->local_euler_angles.x > 89.0f) transform->local_euler_angles.x = 89.0f;
			if (transform->local_euler_angles.x < -89.0f) transform->local_euler_angles.x = -89.0f;

			if (Luxia::Input::GetScrollOffset().y != 0) {
				speed = glm::fclamp(speed * (Luxia::Input::GetScrollOffset().y > 0 ? 2.0f : 0.5f), 0.5f, 256.0f);
			}

			last_mouseX = mouseX;
			last_mouseY = mouseY;
		}
	};

}