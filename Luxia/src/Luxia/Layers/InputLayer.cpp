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
			event_manager.KeyPressed(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<KeyReleaseEvent>([&](KeyReleaseEvent& event) {
			event_manager.KeyReleased(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseButtonPressEvent>([&](MouseButtonPressEvent& event) {
			event_manager.MouseButtonPressed(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseButtonReleaseEvent>([&](MouseButtonReleaseEvent& event) {
			event_manager.MouseButtonReleased(event.GetKeyCode());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseMoveEvent>([&](MouseMoveEvent& event) {
			event_manager.SetMousePosition(event.GetX(), event.GetY());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<MouseScrollEvent>([&](MouseScrollEvent& event) {
			event_manager.MouseScrolled(event.GetX(), event.GetY());
			return shouldConsume; // Check each event type and update input
			});

		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& event) {
			event_manager.windowSize = glm::vec2(event.GetX(), event.GetY());
			return shouldConsume; // Check each event type and update input
			});
	}
}
