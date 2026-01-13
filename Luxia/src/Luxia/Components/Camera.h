#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Platform/AssetCreation.h"
#include <iostream>

#include "Component.h"

#include "Luxia/Scene/Scene.h"
#include "Luxia/Rendering/API/IRenderer.h"
#include "Luxia/Components/Transform.h"

namespace Luxia::Components {
	struct LUXIA_API Camera : public Component {
	public:
		Camera(int m_width, int m_height) {
			width = m_width;
			height = m_height;
			camera = Platform::Assets::CreateCamera(width, height);
		}

		std::shared_ptr<ITexture> Render(const std::shared_ptr<Luxia::Scene>& scene, const std::shared_ptr<Luxia::Rendering::IRenderer>& rend) {
			camera->UpdateMatrix(transform->position, transform->GetRotVec(), FOVdeg, width, height, nearPlane, farPlane);
			return camera->Render(scene, rend, useSkybox, width, height, clearColor);
		}

		float FOVdeg = 70.0f;
		float nearPlane = 0.03f, farPlane = 1000.0f;
		int width = 1920, height = 1080;
		bool main = false;
		bool useSkybox = true;

		glm::vec4 clearColor = glm::vec4(0.3f, 0.5f, 0.4f, 1.0f);
	
		const ICamera* GetCamera() const { return camera.get(); }

		virtual void OnInspectorDraw() override {
			ImGui::DragFloat("FOV", &FOVdeg, 0.1f, 0.0f, 240.0f);
			ImGui::DragFloat("Near Plane", &nearPlane, 0.03f);
			ImGui::DragFloat("Far Plane", &farPlane, nearPlane + 0.01f);
			ImGui::DragInt("Width", &width, 1);
			ImGui::DragInt("Height", &height, 1);
			ImGui::Checkbox("Main", &main);
			ImGui::Checkbox("UseSkybox", &useSkybox);
			ImGui::Spacing();
			ImGui::ColorPicker4("Clear Color", &clearColor.r);
		}
		
	private:
		std::shared_ptr<ICamera> camera = nullptr;
	};
}