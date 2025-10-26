#pragma once

#include "Layer.h"
#include <iostream>
#include "Log.h"

namespace Luxia::Layers {
	class TestLayer : public Luxia::Layer {
	public:
		virtual void OnAttach() override {
			LX_CORE_WARN("TestLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("TestLayer Detached");
		}
		virtual void OnUpdate() override;

		virtual void OnEvent() override {
			std::cout << "TestLayer Event!" << std::endl;
		}
	};
}