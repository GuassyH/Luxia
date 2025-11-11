#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/ICamera.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API Camera : public Component {
		std::shared_ptr<ICamera> camera;

		// Strech fill or fit

		bool main = false;

		Camera(std::shared_ptr<ICamera> m_camera) :
			camera(m_camera) {
		}
	};
}