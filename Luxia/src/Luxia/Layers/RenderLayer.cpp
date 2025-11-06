#include "lxpch.h"
#include "RenderLayer.h"

namespace Luxia::Layers {


	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");

	}
	void RenderLayer::OnDetach() {
		LX_CORE_WARN("RenderLayer Detached");
	}
	void RenderLayer::OnUpdate() {
		cam->UpdateMatrix();
	}
	void RenderLayer::OnRender() {
		cam->Render();
	}
	void RenderLayer::OnEvent(Event& e) {

	}
}
