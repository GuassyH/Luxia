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

    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultSphere = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultCube = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultPlane = nullptr;


    void ResourceManager::Init() {
        NullShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/null.frag", "C:/dev/Luxia/Luxia/resources/shaders/null.vert");
        NullMaterial = Platform::Assets::CreateMaterial(NullShader);

        DefaultLitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_lit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_lit.vert");
        DefaultLitMaterial = Platform::Assets::CreateMaterial(DefaultLitShader);

        DefaultUnlitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_unlit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_unlit.vert");
        DefaultUnlitMaterial = Platform::Assets::CreateMaterial(DefaultUnlitShader);


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

    }

    void ResourceManager::Cleanup() {
        // Meshes
        DefaultSphere->Unload();
        DefaultCube->Unload();
        DefaultPlane->Unload();
    
        DefaultSphere.reset();
        DefaultCube.reset();
        DefaultPlane.reset();
        
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

        NullMaterial->Unload();
        NullShader->Unload();
        NullShader->Delete();
        DefaultUnlitMaterial.reset();
        DefaultUnlitShader.reset();
    }

}