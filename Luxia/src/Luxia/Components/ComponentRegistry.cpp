#include "lxpch.h"
#include "ComponentRegistry.h"


namespace Luxia {

    std::vector<ComponentInfo> componentRegistry = {

        LX_CREATE_COMPONENT_INFO(Components::Camera, "Camera", 1920, 1080),
        LX_CREATE_COMPONENT_INFO(Components::MeshRenderer, "Mesh Renderer"),
        LX_CREATE_COMPONENT_INFO(Components::Light, "Light"),
        LX_CREATE_COMPONENT_INFO(Components::RigidBody, "RigidBody"),
        // Add more here
    };
}
