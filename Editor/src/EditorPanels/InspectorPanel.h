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
	private:
		void RenderMesh(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);
		void RenderMaterial(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);
		void RenderEntity(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene, Luxia::GUID e_guid);
		void RenderShader(Editor::Layers::EditorLayer* editorLayer, Luxia::GUID guid);

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, void>
			PasteAsset(Editor::Layers::EditorLayer* editorLayer, WeakPtrProxy<T>& asset_to_assign) {
			std::string paste = SystemFuncs::PasteFromClipboard();
			Luxia::GUID guid(std::strtoull(paste.c_str(), nullptr, 10));
			if (editorLayer->GetAssetManager()->HasAsset<T>(guid))
				asset_to_assign = editorLayer->GetAssetManager()->GetAsset<T>(guid);
			else
				LX_ERROR("Tried to assign invalid asset: {}", (uint64_t)guid);
		}

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, void>
			DrawPasteField(Editor::Layers::EditorLayer* editorLayer, WeakPtrProxy<T>& asset_to_assign, const char* label) {
			std::ostringstream paste_id; paste_id << "##" << label;
			ImGui::PushID(paste_id.str().c_str());
			if (ImGui::Button("Paste")) {
				PasteAsset<T>(editorLayer, asset_to_assign);
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

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, bool>
			DrawDropField(Editor::Layers::EditorLayer* editorLayer, WeakPtrProxy<T>& asset_to_assign, const char* label) {
			bool changed = false;
			std::ostringstream txt; txt << label << ": ";
			std::string field_text = txt.str();

			std::string buttonid = (asset_to_assign ? std::string(asset_to_assign->name) : std::string("none")) + "##selectable" + std::string(label);
			std::string popupid = std::string("DrawDropFieldPopup##DDFP") + std::string(asset_to_assign.get() ? std::string(asset_to_assign->name) : std::string("none"));

			// Draw the “input box” style selectable on the right
			ImGui::Text("%s", field_text.c_str());
			ImGui::SameLine();

			float fieldWidth = ImGui::GetContentRegionAvail().x - (ImGui::GetWindowWidth() * 0.25f);
			float bright = 2.5f;

			// Push styles
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f * bright, 0.1f * bright, 0.13f * bright, 1.0f));    // base background
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.12f * bright, 0.12f * bright, 0.16f * bright, 1.0f)); // hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f * bright, 0.2f * bright, 0.28f * bright, 1.0f)); // active
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

			// Draw the “field” as an invisible button (acts like a clickable box)
			ImGui::Button(buttonid.c_str(), ImVec2(fieldWidth, 0));

			ImGui::PushID(popupid.c_str());
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Copy", nullptr, nullptr)) {
					if (asset_to_assign)
						SystemFuncs::CopyToClipboard(std::to_string((uint64_t)asset_to_assign->guid));
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Paste", nullptr, nullptr)) {
					PasteAsset<T>(editorLayer, asset_to_assign);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Clear", nullptr, nullptr)) {
					asset_to_assign = nullptr;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			else {
				if (ImGui::IsItemHovered()) {
					if (ImGui::BeginTooltip()) {
						if(asset_to_assign)
							if (editorLayer->asset_thumbnails.contains(asset_to_assign->guid))
								ImGui::Image((ImTextureRef)editorLayer->asset_thumbnails.at(asset_to_assign->guid)->texID, ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));

						ImGui::EndTooltip();
					}
				}
			}
			ImGui::PopID();

			// Recieve Payload
			if (ImGui::BeginDragDropTarget()) {
				if (const auto payload = ImGui::AcceptDragDropPayload("ASSET_ITEM")) {
					LX_CORE_ASSERT(payload->DataSize == sizeof(uint64_t));
					Luxia::GUID payloadGUID = *static_cast<uint64_t*>(payload->Data);

					if (editorLayer->GetAssetManager()->HasAsset<T>(payloadGUID)) {
						asset_to_assign = editorLayer->GetAssetManager()->GetAsset<T>(payloadGUID);
						changed = true;
					}
					else {
						LX_ERROR("Inspector Panel: AssetManager does not contain Asset with guid - {}", (uint64_t)payloadGUID);
					}
				}

				ImGui::EndDragDropTarget();
			}


			// Pop styles
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			return changed;
		}

	};
}
