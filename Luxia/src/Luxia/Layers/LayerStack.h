#pragma once

#include "Layer.h"
#include "Luxia/Core/Core.h"

namespace Luxia {
	class LUXIA_API LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() = default;
		
		std::vector<std::shared_ptr<Luxia::Layer>> m_Layers;

		void PushLayer(std::shared_ptr<Luxia::Layer>& layer, 
			const std::shared_ptr<Luxia::EventHandler>& handler,
			const std::shared_ptr<Luxia::ProjectManager>& manager,
			const std::shared_ptr<Luxia::Rendering::IRenderer>& m_renderer) {
			m_Layers.push_back(layer);
			layer->SetDeps(handler, manager, m_renderer);
			layer->OnAttach();
		}

	};
}