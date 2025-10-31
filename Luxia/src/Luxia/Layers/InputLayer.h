#pragma once

#include "Luxia/Layer.h" 
#include "Luxia/InputSystem.h"

namespace Luxia
{
	class LUXIA_API InputLayer : public Layer
	{
	public:
		InputLayer() = default;
		virtual ~InputLayer() = default;
		
		virtual void OnAttach() override {
			LX_WARN("InputLayer Attached");
		}
		virtual void OnDetach() override {
			LX_WARN("InputLayer Detached");
		}
		virtual void OnUpdate() override {
			// Update logic here
		}

		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);
			
			Input::InputSytem& input = Input::InputSytem::Get();

			dispatcher.Dispatch<KeyPressEvent>([&](KeyPressEvent& event) {
				input.KeyPressed(event.GetKeyCode());
				return false; // Not consumed
				});

			dispatcher.Dispatch<KeyReleaseEvent>([&](KeyReleaseEvent& event) {
				input.KeyReleased(event.GetKeyCode());
				return false; // Not consumed
				});

			dispatcher.Dispatch<MouseButtonPressEvent>([&](MouseButtonPressEvent& event) {
				input.MouseButtonPressed(event.GetKeyCode());
				return false; // Not consumed
				});

			dispatcher.Dispatch<MouseButtonReleaseEvent>([&](MouseButtonReleaseEvent& event) {
				input.MouseButtonReleased(event.GetKeyCode());
				return false; // Not consumed
				});


			// Handle input events here
			// LX_CORE_TRACE("InputLayer received event: {}", e.GetDebug());
		}
	};
}