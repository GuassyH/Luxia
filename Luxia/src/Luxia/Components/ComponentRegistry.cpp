#include "lxpch.h"
#include "ComponentRegistry.h"


namespace Luxia {

    std::vector<ComponentInfo> componentRegistry = {
    { "Camera Component",
      [](Components::Transform* t) { return t->HasComponent<Components::Camera>(); },
      [](Components::Transform* t) { t->AddComponent<Components::Camera>(1920, 1080); }
    },
    { "Mesh Renderer Component",
      [](Components::Transform* t) { return t->HasComponent<Components::MeshRenderer>(); },
      [](Components::Transform* t) { t->AddComponent<Components::MeshRenderer>(); }
    },
        // Add more here
    };
};