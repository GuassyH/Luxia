#pragma once

#include "IEditorPanel.h"
#include "Luxia/Asset/Asset.h"
#include "EditorLayer.h"

namespace Editor::Panels {
	class InspectorPanel : public Editor::IEditorPanel {
	public:
		virtual void Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;



		std::string PasteFromClipboard();
	private:
		void RenderMesh(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);
		void RenderMaterial(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);
		void RenderEntity(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, Luxia::GUID e_guid);
		void RenderShader(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);

		template <typename T>
		void DrawPasteField(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<T>& asset_to_assign, const char* label) {
			std::ostringstream paste_id; paste_id << "##" << label;
			ImGui::PushID(paste_id.str().c_str());
			if (ImGui::Button("Paste")) {
				std::string paste = PasteFromClipboard();
				Luxia::GUID guid(std::strtoull(paste.c_str(), nullptr, 10));
				if (editorLayer->GetAssetManager()->HasAsset<T>(guid))
					asset_to_assign = editorLayer->GetAssetManager()->GetAsset<T>(guid);
				else
					LX_ERROR("Tried to assign invalid asset: {}", (uint64_t)guid);
			}
			ImGui::PopID();

			std::ostringstream txt; txt << label << ": ";
			if (asset_to_assign)
				txt << asset_to_assign->name;
			else
				txt << "nullptr";
			std::string field_label = txt.str();
			std::string field_hint = asset_to_assign ? std::to_string(asset_to_assign->guid) : "none";

			ImGui::SameLine();
			ImGui::Text(field_label.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("%s GUID", label);
				ImGui::Separator();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), field_hint.c_str());
				ImGui::EndTooltip();
			}
		}
	};
}
