#pragma once

#include "Layer.h"
#include <iostream>
#include "Log.h"

#include "Events/MouseEvent.h"

namespace Luxia::Layers {
	class TestLayer : public Luxia::Layer {
	public:
		TestLayer() = default;
		virtual ~TestLayer() = default;

		virtual void OnAttach() override {
			LX_CORE_WARN("TestLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_ERROR("TestLayer Detached");
		}
		virtual void OnUpdate() override;

		virtual void OnEvent(Luxia::Event& event) override {
			LX_CORE_TRACE("TestLayer Event Occurred");

			Luxia::EventDispatcher dispatcher(event);

			dispatcher.Dispatch<Luxia::MouseMoveEvent>([](Luxia::MouseMoveEvent& e) {
				LX_CORE_TRACE(e.GetDebug());
				return true;
				});
		}
	};
}

