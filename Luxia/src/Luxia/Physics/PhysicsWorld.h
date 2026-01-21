#pragma once

#include "Luxia/Core/Core.h"
#include "glm/glm.hpp"


namespace Luxia::Physics {

	struct PhysicsWorldDesc {
		glm::vec3 gravity = glm::vec3(0.0f, -9.82f, 0.0f);
	};

	class PhysicsWorld {
	public:

	};

}