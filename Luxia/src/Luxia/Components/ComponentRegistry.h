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

#define LX_REGISTER_COMPONENT(TYPE, COMP_NAME, ...) \
    ::Luxia::componentRegistry.push_back({ COMP_NAME, \
        [](Components::Transform* t){ return t->HasComponent<TYPE>(); }, \
        [](Components::Transform* t){ t->AddComponent<TYPE>(__VA_ARGS__); } \
    });

#define LX_CREATE_COMPONENT_INFO(TYPE, COMP_NAME, ...) \
    ::Luxia::ComponentInfo{ COMP_NAME, \
        [](Components::Transform* t){ return t->HasComponent<TYPE>(); }, \
        [](Components::Transform* t){ t->AddComponent<TYPE>(__VA_ARGS__); } \
    }