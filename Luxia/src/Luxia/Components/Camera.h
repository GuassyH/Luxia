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
		Camera(int width, int height) {
			camera = Platform::Assets::CreateCamera(width, height);
		}

		std::shared_ptr<ITexture> Render(const std::shared_ptr<Luxia::Scene>& scene, const std::shared_ptr<Luxia::Rendering::IRenderer>& rend) {
			camera->UpdateMatrix(transform->position, transform->GetRotVec());
			return camera->Render(scene, rend);
		}

		glm::vec4 clearColor = glm::vec4(0.3f, 0.5f, 0.4f, 1.0f);
		bool main = false;
	private:
		std::shared_ptr<ICamera> camera;
	};
}