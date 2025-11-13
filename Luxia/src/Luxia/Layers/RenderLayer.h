#pragma once

#include "Layer.h"
#include "Luxia/Asset/ModelAsset.h"


namespace Luxia::Layers
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		RenderLayer() = default;
		~RenderLayer() = default;
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e) override;
	};
}