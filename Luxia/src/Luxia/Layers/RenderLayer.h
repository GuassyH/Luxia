#pragma once

#include "Layer.h"

namespace Luxia::Layers
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		RenderLayer() = default;
		virtual ~RenderLayer() = default;
		
		virtual void OnAttach() override {
			LX_CORE_WARN("RenderLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("RenderLayer Detached");
		}
		virtual void OnUpdate() override {
			// Rendering logic here
		}
		virtual void OnRender() override {

		}
		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);

			// Handle rendering events here
			// LX_CORE_TRACE("RenderLayer received event: {}", e.GetDebug());
		}
	};
}