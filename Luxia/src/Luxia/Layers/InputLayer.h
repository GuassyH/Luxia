#pragma once

#include "Luxia/Layer.h" 

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
			
			// Handle input events here
			// LX_CORE_TRACE("InputLayer received event: {}", e.GetDebug());
		}
	};
}