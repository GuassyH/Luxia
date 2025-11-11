#include "lxpch.h"
#include "EventLayer.h"

namespace Luxia::Layers {


	void EventLayer::OnAttach() {
		LX_CORE_WARN("EventLayer Attached");
	}
	void EventLayer::OnDetach() {
		LX_CORE_WARN("EventLayer Detached");
	}
	void EventLayer::OnUpdate() {

	}
	void EventLayer::OnRender() {

	}
	void EventLayer::OnEvent(Event& e) {
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
			return false; // Check each event type and update input
			});
	
	}
}
