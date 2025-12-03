#include "lxpch.h"
#include "UILayer.h"


namespace Luxia::Layers {

	void UILayer::OnAttach() {
		LX_CORE_WARN("UILayer Attached");
	}
	void UILayer::OnDetach() {
		LX_CORE_WARN("UILayer Detached");
	}
	void UILayer::OnUpdate() {

	}
	void UILayer::OnRender() {

	}
	void UILayer::OnEvent(Event& e) {

	}
}
