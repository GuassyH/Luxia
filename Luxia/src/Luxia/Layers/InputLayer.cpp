#include "lxpch.h"
#include "InputLayer.h"

namespace Luxia::Layers {


	void InputLayer::OnAttach() {
		LX_CORE_WARN("InputLayer Attached");
	}
	void InputLayer::OnDetach() {
		LX_CORE_WARN("InputLayer Detached");
	}
	void InputLayer::OnUpdate() {

	}
	void InputLayer::OnRender() {

	}
	void InputLayer::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		bool shouldConsume = true;

		dispatcher.Dispatch<KeyPressEvent>([&](KeyPressEvent& event) {
			input.KeyPressed(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<KeyReleaseEvent>([&](KeyReleaseEvent& event) {
			input.KeyReleased(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseButtonPressEvent>([&](MouseButtonPressEvent& event) {
			input.MouseButtonPressed(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseButtonReleaseEvent>([&](MouseButtonReleaseEvent& event) {
			input.MouseButtonReleased(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseMoveEvent>([&](MouseMoveEvent& event) {
			input.SetMousePosition(event.GetX(), event.GetY());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseScrollEvent>([&](MouseScrollEvent& event) {
			input.MouseScrolled(event.GetX(), event.GetY());
			return shouldConsume; // Check each event type and update input
			});

	}
}
