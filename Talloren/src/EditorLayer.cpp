#include "EditorLayer.h"
#include "EditorPanels/GameViewport.h"

namespace Talloren::Layers {
	void EditorLayer::OnAttach() {
		LX_CORE_WARN("EditorLayer Attached");
		ImGui::SetCurrentContext(renderer->GetUIRenderer()->GetContext());

		PushPanel(std::make_shared<Talloren::Editor::Panel::GameViewport>());
	}
	void EditorLayer::OnDetach() {
		LX_CORE_WARN("EditorLayer Detached");
	}
	void EditorLayer::OnUpdate() {

	}
	void EditorLayer::OnRender() {
		std::shared_ptr<Luxia::Rendering::IUIRenderer> uiRenderer = renderer->GetUIRenderer();


		static bool opt_fullscreen = true;
		static bool dockspaceOpen = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			// Temp size adjustment to account for top bar
			ImVec2 workPos = viewport->WorkPos;
			ImVec2 workSize = viewport->WorkSize;
			ImGui::SetNextWindowPos(workPos);
			ImGui::SetNextWindowSize(workSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus;

		}

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);


		// Create the dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("Editor Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		}

		// RENDER WINDOWS
		for (auto panel : panels) {
			panel->Render(this);
		}

		ImGui::End();


	}

	void EditorLayer::OnEvent(Luxia::Event& e) {
		// FORWARD EVENT WINDOWS
		for (auto panel : panels) {
			panel->OnEvent(e);
		}
	}

}