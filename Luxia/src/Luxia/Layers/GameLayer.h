#pragma once

#include "Layer.h"

namespace Luxia::Layers {
	class LUXIA_API GameLayer : public Layer {
	public:
		GameLayer() = default;
		virtual ~GameLayer() = default;

		virtual void OnAttach() override {
			LX_CORE_WARN("GameLayer Attached");

		}
		virtual void OnDetach() override {
			LX_CORE_WARN("GameLayer Detached");
		}
		virtual void OnUpdate() override {
			// Rendering logic here
			if (Input::GetScrollOffset().y != 0) {
				LX_CORE_TRACE("Scroll: {}", Input::GetScrollOffset().y);
			}
		}
		virtual void OnRender() override {

		}
		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);
		}
	};
}