#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Platform/AssetCreation.h"
#include <iostream>

#include "Component.h"

#include "Luxia/Scene.h"
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
			return camera->Render(scene, rend, width, height);
		}

		float FOVdeg = 70.0f;
		float nearPlane = 0.03f, farPlane = 1000.0f;
		int width = 1920, height = 1080;

		glm::vec4 clearColor = glm::vec4(0.3f, 0.5f, 0.4f, 1.0f);
		bool main = false;
	private:
		std::shared_ptr<ICamera> camera;
	};
}