#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Light.h"
#include "RigidBody.h"

#include "Colliders/Collider.h"
#include "Colliders/BoxCollider.h"
#include "Colliders/SphereCollider.h"

namespace Luxia {
    struct LUXIA_API ComponentInfo {
        std::string name;
        std::function<bool(Components::Transform*)> hasFunc;
        std::function<void(Components::Transform*)> addFunc;
        std::function<void(Components::Transform*)> removeFunc;
        std::function<Components::Component*(Components::Transform*)> getFunc;
    };

    extern LUXIA_API std::vector<ComponentInfo> componentRegistry;

};

#define LX_REGISTER_COMPONENT(TYPE, COMP_NAME, ...) \
    ::Luxia::componentRegistry.push_back({ COMP_NAME, \
        [](Components::Transform* t){ return t->HasComponent<TYPE>(); }, \
        [](Components::Transform* t){ t->AddComponent<TYPE>(__VA_ARGS__); }, \
        [](Components::Transform* t) { t->RemoveComponent<TYPE>(); }, \
        [](Components::Transform* t) { return t->TryGetComponent<TYPE>(); } \
    });

#define LX_CREATE_COMPONENT_INFO(TYPE, COMP_NAME, ...) \
    ::Luxia::ComponentInfo{ COMP_NAME, \
        [](Components::Transform* t){ return t->HasComponent<TYPE>(); }, \
        [](Components::Transform* t){ t->AddComponent<TYPE>(__VA_ARGS__); }, \
        [](Components::Transform* t) { t->RemoveComponent<TYPE>(); }, \
        [](Components::Transform* t) { return t->TryGetComponent<TYPE>(); } \
    }