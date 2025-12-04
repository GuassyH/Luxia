#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Editor::Panel {
	void InspectorPanel::Init() {
		LX_INFO("Editor - Inspector Panel: Init");
	}
	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel");

		if (!editorLayer->is_entity_selected) { ImGui::End(); return; }

		auto t = scene->TryGetFromEntity<Luxia::Components::Transform>(editorLayer->selected_entity);

		if (t) {
			t->OnInspectorDraw();
		}

		ImGui::End();
	}
	void InspectorPanel::Unload() {

	}

	void InspectorPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}