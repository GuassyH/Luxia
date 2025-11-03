#pragma once

#include "Luxia/Core.h"
#include "glm/glm.hpp"

namespace Luxia::Rendering {
	struct LUXIA_API Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoord;
	};
}