#pragma once

#include "Luxia/Core/Core.h"

#include "Luxia/Events/EventDefs.h"
#include "glm/vec2.hpp"

#include <unordered_set>
#include <iostream>

namespace Luxia {
	class LUXIA_API EventManager {
	private:
		EventManager() = default;
	public:
		static EventManager& Get() {
			static EventManager instance;
			return instance;
		}

		// ==== INPUT ====

		// Store an unordered set of the keys (O(1) lookup time)
		std::unordered_set<int> pressedKeys;
		std::unordered_set<int> justPressedKeys;
		std::unordered_set<int> justReleasedKeys;

		std::unordered_set<int> pressedMouseButtons;
		std::unordered_set<int> justPressedMouseButtons;
		std::unordered_set<int> justReleasedMouseButtons;

		glm::vec2 scrollOffset = glm::vec2(0.0f);
		glm::vec2 mousePosition = glm::vec2(0.0f);

		// ==== SCREEN ====
		glm::vec2 windowSize = glm::vec2(0.0f);
		glm::vec2 monitorSize = glm::vec2(0.0f);

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

		void Clear() { // Is called at the end of the frame

			// Reset scroll, since scroll is an instant thing
			scrollOffset = glm::vec2(0.0f);

			// Clear just pressed keys, keys that were pressed THIS frame should not carry on
			justPressedKeys.clear();
			justPressedMouseButtons.clear();

			justReleasedKeys.clear();
			justReleasedMouseButtons.clear();
		}
	};

}

namespace Luxia::Input {
	// Easy to call functions, just say "if(Input::IsKeyPressed(YOUR_KEY))"
	inline LUXIA_API bool AnyKeyPressed() {
		auto& input = EventManager::Get();
		return !input.pressedKeys.empty();
	}
	inline LUXIA_API bool IsKeyPressed(int keycode) {
		auto& input = EventManager::Get();
		return input.pressedKeys.contains(keycode);
	}
	inline LUXIA_API bool IsKeyJustPressed(int keycode) {
		auto& input = EventManager::Get();
		return input.justPressedKeys.contains(keycode);
	}
	inline LUXIA_API bool IsKeyJustReleased(int keycode) {
		auto& input = EventManager::Get();
		return input.justReleasedKeys.contains(keycode);
	}

	inline LUXIA_API bool AnyMouseButtonPressed() {
		auto& input = EventManager::Get();
		return !input.pressedMouseButtons.empty();
	}
	inline LUXIA_API bool IsMouseButtonPressed(int button) {
		auto& input = EventManager::Get();
		return input.pressedMouseButtons.contains(button);
	}
	inline LUXIA_API bool IsMouseButtonJustPressed(int button) {
		auto& input = EventManager::Get();
		return input.justPressedMouseButtons.contains(button);
	}
	inline LUXIA_API bool IsMouseButtonJustReleased(int button) {
		auto& input = EventManager::Get();
		return input.justReleasedMouseButtons.contains(button);
	}

	// Self explanatory
	inline LUXIA_API glm::vec2 GetMousePosition() {
		auto& input = EventManager::Get();
		return input.mousePosition;
	}

	// x is left & right, y is up & down
	inline LUXIA_API glm::vec2 GetScrollOffset() {
		auto& input = EventManager::Get();
		return input.scrollOffset;
	}
}

namespace Luxia::Screen {
	inline LUXIA_API glm::vec2 GetWindowSize() {
		auto& screen = EventManager::Get();
		return screen.windowSize;
	}
	inline LUXIA_API glm::vec2 GetMonitorSize() {
		auto& screen = EventManager::Get();
		return screen.monitorSize;
	}
}
