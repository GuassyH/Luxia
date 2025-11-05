#pragma once

#include "Layer.h"

namespace Luxia::Layers {
	class LUXIA_API GameLayer : public Layer {
	public:
		GameLayer() = default;
		~GameLayer() = default;
		GameLayer(
			const std::shared_ptr<EventHandler>& handler,
			const std::shared_ptr<ProjectManager>& manager,
			const std::shared_ptr<Rendering::IRenderer>& m_renderer)
			: Layer(handler, manager, m_renderer) {
		}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e) override;
	};
}