#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/ICamera.h"
#include <iostream>

namespace Luxia::Components {
	struct LUXIA_API Camera {
		std::shared_ptr<ICamera> camera;

		Camera(std::shared_ptr<ICamera> m_camera) :
			camera(m_camera) {
		}
	};
}