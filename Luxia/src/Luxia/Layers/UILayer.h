#pragma once

#include "Layer.h"
#include "Luxia/Core/Core.h"

namespace Luxia::Layers {
	class LUXIA_API UILayer : public Luxia::Layer
	{
	public:
		UILayer() = default;
		~UILayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;
	};
}

