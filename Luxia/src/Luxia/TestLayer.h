#pragma once

#include "Layer.h"
#include <iostream>

namespace Luxia::Layers {
	class TestLayer : public Luxia::Layer {
	public:
		virtual void OnAttach() override {
			std::cout << "TestLayer Attached!" << std::endl;
		}
		virtual void OnDetach() override {
			std::cout << "TestLayer Detached!" << std::endl;
		}

		virtual void OnUpdate() override;

		virtual void OnEvent() override {
			std::cout << "TestLayer Event!" << std::endl;
		}
	};
}