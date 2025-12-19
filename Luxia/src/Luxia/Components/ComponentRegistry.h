#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"

namespace Luxia {
	struct ComponentInfo {
		std::string name;
		std::function<bool(Components::Transform*)> hasFunc;
		std::function<void(Components::Transform*)> addFunc;
	};

	extern std::vector<ComponentInfo> componentRegistry;
};