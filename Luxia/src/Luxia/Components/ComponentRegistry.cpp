#include "lxpch.h"
#include "ComponentRegistry.h"


namespace Luxia {

    std::vector<ComponentInfo> componentRegistry = {

        LX_CREATE_COMPONENT_INFO(Components::Camera, "Camera", 1920, 1080),
        LX_CREATE_COMPONENT_INFO(Components::MeshRenderer, "Mesh Renderer"),
        // Add more here
    };
}
