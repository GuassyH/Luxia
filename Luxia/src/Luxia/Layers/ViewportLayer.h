#pragma once

#include "Luxia/Layers/Layer.h"
#include "Luxia/Core/Core.h"

namespace Luxia::Layers {
	class LUXIA_API ViewportLayer : public Luxia::Layer {
	public:
		std::shared_ptr<Luxia::IShader> fs_shader;
		Luxia::Mesh fs_quad;

		ViewportLayer() { name = "Viewport Layer"; }
		~ViewportLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;

		bool RenderCameraToVP(Luxia::RenderCameraEvent& e);
	};
}

