#pragma once

#include "Layer.h"

namespace Luxia {
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() = default;
		
		std::vector<std::shared_ptr<Luxia::Layer>> m_Layers;

		void PushLayer(std::shared_ptr<Luxia::Layer> layer) {
			m_Layers.push_back(layer);
			layer->OnAttach();
		}

	};
}