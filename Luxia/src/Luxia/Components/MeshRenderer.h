#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API MeshRenderer : public Component {
		// Cant be weak ptr since when it goes out of scope the weakptr goes null
		std::shared_ptr<Luxia::Mesh> mesh = nullptr;
		std::shared_ptr<Luxia::IMaterial> material = nullptr;

		MeshRenderer() { name = "Mesh Renderer"; }
		MeshRenderer(const std::shared_ptr<Luxia::Mesh>& m_mesh, const std::shared_ptr<IMaterial>& m_material) {
			mesh = m_mesh;
			material = m_material;
			name = "Mesh Renderer";
		}

		GUID meshguid = GUID(0);
		GUID matguid = GUID(0);

		char meshguidbuff[255] = "";
		char matguidbuff[255] = "";
		virtual void OnInspectorDraw() override {
			int flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_AutoSelectAll | 
				ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsDecimal;


			std::ostringstream mesht; mesht << "Mesh: ";
			if (!mesh)
				mesht << "nullptr";
			else
				mesht << mesh->name;
			
			ImGui::Text(mesht.str().c_str());
			
			ImGui::SameLine();
			if(ImGui::InputTextWithHint("##MeshBar", (char*)(uint64_t)meshguid, meshguidbuff, sizeof(meshguidbuff), flags)) {
				meshguid = (uint64_t)meshguidbuff;
				memset(meshguidbuff, 0, 255);
			}

			std::ostringstream matt; matt << "Material: ";
			if (!material)
				matt << "nullptr";
			else
				matt << material->name;

			ImGui::Text(matt.str().c_str());


			ImGui::SameLine();
			if (ImGui::InputTextWithHint("##MaterialBar", (char*)(uint64_t)matguid, matguidbuff, sizeof(matguidbuff), flags)) {
				matguid = (uint64_t)matguidbuff;
				memset(matguidbuff, 0, 255);
			}
		}
	};
}