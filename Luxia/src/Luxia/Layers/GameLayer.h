#pragma once

#include "Layer.h"

namespace Luxia::Layers {
	class LUXIA_API GameLayer : public Layer {
	public:
		GameLayer() = default;
		~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e) override;
	};
}