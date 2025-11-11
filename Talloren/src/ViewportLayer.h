#pragma once

#include "Luxia.h"

namespace Talloren::Layer {
	class ViewportLayer : public Luxia::Layer {
	public:
		std::shared_ptr<Luxia::IShader> fs_shader;
		Luxia::Mesh fs_quad;

		ViewportLayer() = default;
		~ViewportLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;

		bool RenderCameraToVP(Luxia::RenderCameraEvent& e);
	};
}

