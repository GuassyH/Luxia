#include "lxpch.h"
#include "ResourceManager.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia {

    std::shared_ptr<Luxia::IShader> ResourceManager::NullShader = nullptr;
    std::shared_ptr<Luxia::IMaterial> ResourceManager::NullMaterial = nullptr;

    std::shared_ptr<Luxia::IShader> ResourceManager::DefaultUnlitShader = nullptr;
    std::shared_ptr<Luxia::IMaterial> ResourceManager::DefaultUnlitMaterial = nullptr;

    std::shared_ptr<Luxia::IShader> ResourceManager::DefaultLitShader = nullptr;
    std::shared_ptr<Luxia::IMaterial> ResourceManager::DefaultLitMaterial = nullptr;

    std::shared_ptr<Luxia::IShader> ResourceManager::DepthOnlyShader = nullptr;
    std::shared_ptr<Luxia::IMaterial> ResourceManager::DepthOnlyMaterial = nullptr;

    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultSphere = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultCube = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultPlane = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultQuad = nullptr;


    void ResourceManager::Init() {
        NullShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/null.frag", "C:/dev/Luxia/Luxia/resources/shaders/null.vert");
        NullMaterial = Platform::Assets::CreateMaterial(NullShader);

        DefaultLitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_lit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_lit.vert");
        DefaultLitMaterial = Platform::Assets::CreateMaterial(DefaultLitShader);

        DefaultUnlitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_unlit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_unlit.vert");
        DefaultUnlitMaterial = Platform::Assets::CreateMaterial(DefaultUnlitShader);

        DepthOnlyShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/depth_only.frag", "C:/dev/Luxia/Luxia/resources/shaders/depth_only.vert");
        DepthOnlyMaterial = Platform::Assets::CreateMaterial(DepthOnlyShader);

        DefaultSphere = [] {
            Constants::Shapes::UVSphere sphere;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                sphere.getVertices(),
                sphere.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();

        DefaultCube = [] {
            Constants::Shapes::Cube cube;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                cube.getVertices(),
                cube.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();

        DefaultPlane = [] {
            Constants::Shapes::Plane plane;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                plane.getVertices(),
                plane.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();


        DefaultQuad = [] {
            std::vector<Luxia::Rendering::Vertex> quad_verts {
	        {{glm::vec3(-1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 0.0f)}},
	        {{glm::vec3(1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 0.0f)}},
	        {{glm::vec3(-1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 1.0f)}},
	        {{glm::vec3(1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 1.0f)}}
            };
            std::vector<unsigned int> quad_inds{0, 1, 2,  1, 3, 2 };
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                quad_verts,
                quad_inds
            );
            mesh->CalculateMesh();
            return mesh;
            }();
    }

    void ResourceManager::Cleanup() {
        // Meshes
        DefaultSphere->Unload();
        DefaultCube->Unload();
        DefaultPlane->Unload();
        DefaultQuad->Unload();

        DefaultSphere.reset();
        DefaultCube.reset();
        DefaultPlane.reset();
        DefaultQuad.reset();
        
        // Materials
        DefaultLitMaterial->Unload();
        DefaultLitShader->Unload();
        DefaultLitShader->Delete();
        DefaultLitMaterial.reset();
        DefaultLitShader.reset();

        DefaultUnlitMaterial->Unload();
        DefaultUnlitShader->Unload();
        DefaultUnlitShader->Delete();
        DefaultUnlitMaterial.reset();
        DefaultUnlitShader.reset();

        DepthOnlyMaterial->Unload();
        DepthOnlyShader->Unload();
        DepthOnlyShader->Delete();
        DepthOnlyMaterial.reset();
        DepthOnlyShader.reset();

        NullMaterial->Unload();
        NullShader->Unload();
        NullShader->Delete();
        DefaultUnlitMaterial.reset();
        DefaultUnlitShader.reset();
    }

}