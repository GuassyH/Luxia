#include "EditorLayer.h"

namespace Talloren::Layers {

	void EditorLayer::OnAttach() {
		LX_CORE_WARN("EditorLayer Attached");
	}
	void EditorLayer::OnDetach() {
		LX_CORE_WARN("EditorLayer Detached");
	}
	void EditorLayer::OnUpdate() {
	}
	void EditorLayer::OnRender() {
	}
	void EditorLayer::OnEvent(Luxia::Event& e) {

	}

}