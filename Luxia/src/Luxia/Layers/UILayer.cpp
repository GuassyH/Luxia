#include "lxpch.h"
#include "UILayer.h"

namespace Luxia::Layers {

	bool opened = true;
	void UILayer::OnAttach() {
		LX_CORE_WARN("UILayer Attached");
	}
	void UILayer::OnDetach() {
		LX_CORE_WARN("UILayer Detached");
	}
	void UILayer::OnUpdate() {

	}
	void UILayer::OnRender() {
		std::shared_ptr<Rendering::IUIRenderer> uiRenderer = renderer->GetUIRenderer();

		// This is FULLY over the ENTIRE window
		ImGui::SetNextWindowSize(ImVec2(window->GetSize().x, window->GetSize().y));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		if (ImGui::Begin("UI Layer", &opened, flags)) {
			uiRenderer->RenderElements();

		}	ImGui::End();

	}
	void UILayer::OnEvent(Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		/* If a camera is rendered do something?
		dispatcher.Dispatch<RenderCameraEvent>([&](RenderCameraEvent& event) {
			size = glm::vec2(event.GetTexture()->GetWidth(), event.GetTexture()->GetHeight());
			return false; // Check each event type and update input
			});
		*/
	}
}
