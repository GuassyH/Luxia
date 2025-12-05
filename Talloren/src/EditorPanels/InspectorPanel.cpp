#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}
	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel");
		if (!editorLayer->is_entity_selected) { ImGui::End(); return; }

		std::ostringstream info; info << "Entity ID: " << (int)editorLayer->selected_entity;
		// Draw GUID information (currently just entity id, but entity should be made into a class)
		ImGui::Text(info.str().c_str());

		// Draw transform component first
		auto t = scene->TryGetFromEntity<Luxia::Components::Transform>(editorLayer->selected_entity);
		if (t) {
			if (ImGui::CollapsingHeader(t->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				t->OnInspectorDraw();
			}
		}

		// Draw other components
		// foreach component, make collapsingheader, draw

		ImGui::End();
	}
	void InspectorPanel::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {

	}

	void InspectorPanel::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		// dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderImage));
	}
}