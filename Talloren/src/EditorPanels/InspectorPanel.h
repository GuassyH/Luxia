#pragma once

#include "IEditorPanel.h"
#include "Luxia/Asset/Asset.h"
#include "EditorLayer.h"

namespace Talloren::Panels {
	class InspectorPanel : public Talloren::IEditorPanel {
	public:
		virtual void Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;
		virtual void Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) override;

		virtual void OnEvent(Luxia::Event& e) override;



		std::string PasteFromClipboard();
	private:
		void RenderMaterial(Talloren::Layers::EditorLayer* editorLayer, Luxia::GUID guid);
		void RenderEntity(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, Luxia::GUID e_guid);

		template <typename T>
		void DrawPasteField(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<T>& asset_to_assign, const char* label) {
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

			std::ostringstream hvr_txt; hvr_txt << label << " GUID";

			ImGui::SameLine();
			ImGui::Text(field_label.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(hvr_txt.str().c_str());
				ImGui::Separator();
				ImGui::TextColored(ImVec4(1, 1, 0, 1), field_hint.c_str());
				ImGui::EndTooltip();
			}
		}
	};
}
