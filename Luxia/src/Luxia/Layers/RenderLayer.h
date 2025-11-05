#pragma once

#include "Layer.h"
#include "Luxia/Assets/ModelAsset.h"


namespace Luxia::Layers
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		RenderLayer() = default;
		~RenderLayer() = default;
		
		std::shared_ptr<ICamera> cam = Luxia::Platform::Assets::CreateCamera(1920, 1080);
		std::shared_ptr<IShader> shader = 
			Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e) override;
	};
}