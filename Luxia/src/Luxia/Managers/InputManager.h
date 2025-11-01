#pragma once


#include "Luxia/Core.h"

#include <unordered_set>
#include "Luxia/Events/EventPCH.h"
#include "glm/glm/common.hpp"
#include "glm/glm.hpp"

namespace Luxia::Input {
	
	class LUXIA_API InputManager {
	private:
		InputManager() = default;
	public:
		static InputManager& Get() {
			static InputManager instance;
			return instance;
		}

		// Store an unordered set of the keys (O(1) lookup time)
		std::unordered_set<int> pressedKeys;
		std::unordered_set<int> justPressedKeys;
		std::unordered_set<int> justReleasedKeys;

		std::unordered_set<int> pressedMouseButtons;
		std::unordered_set<int> justPressedMouseButtons;
		std::unordered_set<int> justReleasedMouseButtons;

		glm::vec2 scrollOffset;
		glm::vec2 mousePosition;

		// KEYBOARD

		void KeyPressed(int keycode) {
			if (!pressedKeys.contains(keycode)) {
				pressedKeys.insert(keycode);
				justPressedKeys.insert(keycode);
			}
		}
		void KeyReleased(int keycode) {
			if (pressedKeys.contains(keycode)) {
				pressedKeys.erase(keycode);
				justReleasedKeys.insert(keycode);
			}
		}

		// MOUSE

		void MouseButtonPressed(int button) {
			if (!pressedKeys.contains(button)) {
				pressedMouseButtons.insert(button);
				justPressedMouseButtons.insert(button);
			}
		}
		void MouseButtonReleased(int button) {
			if (pressedMouseButtons.contains(button)) {
				pressedMouseButtons.erase(button);
				justReleasedMouseButtons.insert(button);
			}
		}
		void SetMousePosition(float x, float y) {
			mousePosition.x = x;
			mousePosition.y = y;
		}

		void MouseScrolled(float xOffset, float yOffset) {
			scrollOffset.x = xOffset;
			scrollOffset.y = yOffset;
		}

		// GENERAL

		void Reset() { // Is called at the end of the frame

			// Reset scroll, since scroll is an instant thing
			scrollOffset = glm::vec2(0.0f);

			// Clear just pressed keys, keys that were pressed THIS frame should not carry on
			justPressedKeys.clear();
			justPressedMouseButtons.clear();

			justReleasedKeys.clear();
			justReleasedMouseButtons.clear();
		}
	};

	// Easy to call functions, just say "if(Input::IsKeyPressed(YOUR_KEY))"
	inline LUXIA_API bool AnyKeyPressed() {
		auto& input = InputManager::Get();
		return !input.pressedKeys.empty();
	}
	inline LUXIA_API bool IsKeyPressed(int keycode) {
		auto& input = InputManager::Get();
		return input.pressedKeys.contains(keycode);
	}
	inline LUXIA_API bool IsKeyJustPressed(int keycode) {
		auto& input = InputManager::Get();
		return input.justPressedKeys.contains(keycode);
	}
	inline LUXIA_API bool IsKeyJustReleased(int keycode) {
		auto& input = Input::InputManager::Get();
		return input.justReleasedKeys.contains(keycode);
	}

	inline LUXIA_API bool AnyMouseButtonPressed() {
		auto& input = InputManager::Get();
		return !input.pressedMouseButtons.empty();
	}
	inline LUXIA_API bool IsMouseButtonPressed(int button) {
		auto& input = InputManager::Get();
		return input.pressedMouseButtons.contains(button);
	}
	inline LUXIA_API bool IsMouseButtonJustPressed(int button) {
		auto& input = InputManager::Get();
		return input.justPressedMouseButtons.contains(button);
	}
	inline LUXIA_API bool IsMouseButtonJustReleased(int button) {
		auto& input = Input::InputManager::Get();
		return input.justReleasedMouseButtons.contains(button);
	}

	// Self explanatory
	inline LUXIA_API glm::vec2 GetMousePosition() {
		auto& input = InputManager::Get();
		return input.mousePosition;
	}

	// x is left & right, y is up & down
	inline LUXIA_API glm::vec2 GetScrollOffset() {
		auto& input = InputManager::Get();
		return input.scrollOffset;
	}
}