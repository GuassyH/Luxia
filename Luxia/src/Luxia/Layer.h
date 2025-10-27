#pragma once

#include "Event.h"

namespace Luxia
{
	class Layer {
		public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Luxia::Event& event) = 0;
	};
}
