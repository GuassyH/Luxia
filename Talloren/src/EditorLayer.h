#pragma once
#include "Luxia.h"

namespace Talloren::Layers {
	class EditorLayer : public Luxia::Layer
	{
	public:
		EditorLayer() = default;
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;
	};
}

