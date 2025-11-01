#pragma once

#include "Layer.h" 

namespace Luxia::Layers
{
	class LUXIA_API InputLayer : public Layer
	{
	private:
		Input::InputManager& input = Input::InputManager::Get();
	public:
		InputLayer() = default;
		virtual ~InputLayer() = default;
		
		virtual void OnAttach() override {
			LX_CORE_WARN("InputLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("InputLayer Detached");
		}
		virtual void OnUpdate() override {

		}
		virtual void OnRender() override {
		
		}
		virtual void OnEvent(Event& e) override {
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
	};
}