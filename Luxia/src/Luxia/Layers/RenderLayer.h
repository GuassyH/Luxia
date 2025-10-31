#pragma once

#include "Luxia/Layer.h"
#include "Luxia/InputSystem.h"

namespace Luxia
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		RenderLayer() = default;
		virtual ~RenderLayer() = default;
		
		virtual void OnAttach() override {
			LX_WARN("RenderLayer Attached");
		}
		virtual void OnDetach() override {
			LX_WARN("RenderLayer Detached");
		}
		virtual void OnUpdate() override {
			// Rendering logic here

		}
		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);
			
			// Handle rendering events here
			// LX_CORE_TRACE("RenderLayer received event: {}", e.GetDebug());
		}
	};
}