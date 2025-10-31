#pragma once


#include "Core.h"

#include "Luxia/Events/Event.h"
#include "Luxia/Events/KeyEvent.h"
#include "Luxia/Events/MouseEvent.h"
#include <unordered_set>

namespace Luxia::Input {
	
	class LUXIA_API InputSytem {
	private:
		InputSytem() = default;
	public:
		std::unordered_set<int> pressedKeys;
		std::unordered_set<int> pressedMouseButton;

		static InputSytem& Get() {
			static InputSytem instance;
			return instance;
		}
		void KeyPressed(int keycode) {
			if (!pressedKeys.contains(keycode)) {
				pressedKeys.insert(keycode);
			}
		}
		void KeyReleased(int keycode) {
			if (pressedKeys.contains(keycode)) {
				pressedKeys.erase(keycode);
			}
		}
		void MouseButtonPressed(int button) {
			if (!pressedKeys.contains(button)) {
				pressedMouseButton.insert(button);
			}
		}
		void MouseButtonReleased(int button) {
			if (pressedMouseButton.contains(button)) {
				pressedMouseButton.erase(button);
			}
		}
	};

	inline LUXIA_API bool IsKeyPressed(int keycode) {
		auto& input = InputSytem::Get();
		return input.pressedKeys.contains(keycode);
	}

	inline LUXIA_API bool IsMouseButtonPressed(int button) {
		auto& input = InputSytem::Get();
		return input.pressedMouseButton.contains(button);
	}

}