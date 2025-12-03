#include "lxpch.h"
#include "UILayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Luxia::Layers {

	void UILayer::OnAttach() {
		LX_CORE_WARN("UILayer Attached");
		renderer->GetUIRenderer()->Init();
	}
	void UILayer::OnDetach() {
		LX_CORE_WARN("UILayer Detached");
	}
	void UILayer::OnUpdate() {

	}
	void UILayer::OnRender() {
		std::shared_ptr<Rendering::IUIRenderer> uiRenderer = renderer->GetUIRenderer();

		uiRenderer->BeginFrame();

		if(ImGui::Begin("UI Layer")) {
			ImGui::Text("Hello from UILayer!");
		}
		ImGui::End();
		uiRenderer->Render();


		uiRenderer->EndFrame();
	}
	void UILayer::OnEvent(Event& e) {

	}
}
