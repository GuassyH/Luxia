#pragma once


#include "Core.h"

#include "Luxia/Events/Event.h"
#include "Luxia/Events/KeyEvent.h"
#include "Luxia/Events/MouseEvent.h"
#include <unordered_set>
#include "lxm/lxm_common.h"

namespace Luxia::Input {
	
	class LUXIA_API InputSytem {
	private:
		InputSytem() = default;
	public:
		static InputSytem& Get() {
			static InputSytem instance;
			return instance;
		}

		std::unordered_set<int> pressedKeys;
		std::unordered_set<int> justPressedKeys;
		std::unordered_set<int> pressedMouseButton;
		std::unordered_set<int> justPressedMouseButton;

		lxm::Vec2 scrollOffset;
		lxm::Vec2 mousePosition;

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
			}
		}

		// MOUSE

		void MouseButtonPressed(int button) {
			if (!pressedKeys.contains(button)) {
				pressedMouseButton.insert(button);
				justPressedMouseButton.insert(button);
			}
		}
		void MouseButtonReleased(int button) {
			if (pressedMouseButton.contains(button)) {
				pressedMouseButton.erase(button);
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
			scrollOffset = lxm::Vec2(0.0f);

			// Clear just pressed keys, keys that were pressed THIS frame should not carry on
			justPressedKeys.clear();
			justPressedMouseButton.clear();
		}
	};

	// If any key is pressed
	inline LUXIA_API bool AnyKeyPressed() {
		auto& input = InputSytem::Get();
		return !input.pressedKeys.empty();
	}
	inline LUXIA_API bool IsKeyPressed(int keycode) {
		auto& input = InputSytem::Get();
		return input.pressedKeys.contains(keycode);
	}
	// If a key was pressed this frame
	inline LUXIA_API bool IsKeyJustPressed(int keycode) {
		auto& input = InputSytem::Get();
		return input.justPressedKeys.contains(keycode);
	}


	// If any mouse button is pressed
	inline LUXIA_API bool AnyMouseButtonPressed() {
		auto& input = InputSytem::Get();
		return !input.pressedMouseButton.empty();
	}
	inline LUXIA_API bool IsMouseButtonPressed(int button) {
		auto& input = InputSytem::Get();
		return input.pressedMouseButton.contains(button);
	}
	// If mouse button was pressed JUST this frame
	inline LUXIA_API bool IsMouseButtonJustPressed(int button) {
		auto& input = InputSytem::Get();
		return input.justPressedMouseButton.contains(button);
	}

	// Self explanatory
	inline LUXIA_API lxm::Vec2 GetMousePosition() {
		auto& input = InputSytem::Get();
		return input.mousePosition;
	}

	// x is left & right, y is up & down
	inline LUXIA_API lxm::Vec2 GetScrollOffset() {
		auto& input = InputSytem::Get();
		return input.scrollOffset;
	}
}