#pragma once

#include "Layer.h" 

namespace Luxia::Layers
{
	class LUXIA_API EventLayer : public Layer {
	private:
		Luxia::EventManager& event_manager = Luxia::EventManager::Get();
	public:
		EventLayer() = default;
		~EventLayer() = default;

		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e) override;
	};
}