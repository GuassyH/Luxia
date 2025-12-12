#include "InspectorPanel.h"
#include "EditorLayer.h"

namespace Talloren::Panel {
	void InspectorPanel::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		LX_INFO("Editor - Inspector Panel: Init");
	}
	void InspectorPanel::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Inspector Panel");
		if (!editorLayer->is_entity_selected) { ImGui::End(); return; }
		if (!scene->runtime_entities.contains(editorLayer->selected_entity)) { ImGui::End(); return; }

		Luxia::Entity& ent = scene->runtime_entities.find(editorLayer->selected_entity)->second;

		std::ostringstream info; info << "Entity ID: " << (uint64_t)editorLayer->selected_entity;
		// Draw GUID information (currently just entity id, but entity should be made into a class)
		ImGui::Text(info.str().c_str());

		// Draw transform component first
		if (ent.transform) {
			if (ImGui::CollapsingHeader(ent.transform->name, ImGuiTreeNodeFlags_DefaultOpen)) {
				ent.transform->OnInspectorDraw();
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