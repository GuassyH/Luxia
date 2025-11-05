#pragma once

#include "Luxia/Core/Core.h"
#include "glm/glm.hpp"

namespace Luxia::Rendering {
	struct LUXIA_API Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};
}