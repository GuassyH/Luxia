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

    std::shared_ptr<Luxia::IShader> ResourceManager::DefaultSkyboxShader = nullptr;
    std::shared_ptr<Luxia::IMaterial> ResourceManager::DefaultSkyboxMaterial = nullptr;

    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultSphere = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultCube = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultPlane = nullptr;
    std::shared_ptr<Luxia::Mesh> ResourceManager::DefaultQuad = nullptr;

    std::vector<std::shared_ptr<Luxia::Assets::Asset>> ResourceManager::ResourcesVector = std::vector<std::shared_ptr<Luxia::Assets::Asset>>();


    void ResourceManager::Init() {
        NullShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/null.frag", "C:/dev/Luxia/Luxia/resources/shaders/null.vert");
        NullShader->guid = GUID(2456434234);
        NullShader->name = "null_shader";

        NullMaterial = Platform::Assets::CreateMaterial(NullShader);
        NullMaterial->guid = GUID(657845365465); 
        NullMaterial->name = "null_material";

        DefaultLitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_lit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_lit.vert");
        DefaultLitShader->guid = GUID(1265423432);
        DefaultLitShader->name = "default_lit_shader";

        DefaultLitMaterial = Platform::Assets::CreateMaterial(DefaultLitShader);
        DefaultLitMaterial->guid = GUID(867435435234);
        DefaultLitMaterial->name = "default_lit_material";

        DefaultUnlitShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_unlit.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_unlit.vert");
        DefaultUnlitShader->guid = GUID(56436538536);
        DefaultUnlitShader->name = "default_unlit_shader";

        DefaultUnlitMaterial = Platform::Assets::CreateMaterial(DefaultUnlitShader);
        DefaultUnlitMaterial->guid = GUID(987656445376645);
        DefaultUnlitMaterial->name = "default_unlit_material";

        DepthOnlyShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/depth_only.frag", "C:/dev/Luxia/Luxia/resources/shaders/depth_only.vert");
        DepthOnlyShader->guid = GUID(43534534735674);
        DepthOnlyShader->name = "depth_only_shader";

        DepthOnlyMaterial = Platform::Assets::CreateMaterial(DepthOnlyShader);
        DepthOnlyMaterial->guid = GUID(78989767556765);
        DepthOnlyMaterial->name = "depth_only_material";

        DefaultSkyboxShader = Platform::Assets::CreateShader("C:/dev/Luxia/Luxia/resources/shaders/default_skybox.frag", "C:/dev/Luxia/Luxia/resources/shaders/default_skybox.vert");
        DefaultSkyboxShader->guid = GUID(2343263346547);
        DefaultSkyboxShader->name = "default_skybox_shader";

        DefaultSkyboxMaterial = Platform::Assets::CreateMaterial(DefaultSkyboxShader);
        DefaultSkyboxMaterial->guid = GUID(856675657563435);
        DefaultSkyboxMaterial->name = "default_skybox_material";


        DefaultSphere = [] {
            Constants::Shapes::UVSphere sphere;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                sphere.getVertices(),
                sphere.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();
        DefaultSphere->guid = GUID(340928349);
        DefaultSphere->name = "sphere_mesh";

        DefaultCube = [] {
            Constants::Shapes::Cube cube;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                cube.getVertices(),
                cube.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();
        DefaultCube->guid = GUID(643234264374456);
        DefaultCube->name = "cube_mesh";

        DefaultPlane = [] {
            Constants::Shapes::Plane plane;
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                plane.getVertices(),
                plane.getIndices()
            );
            mesh->CalculateMesh();
            return mesh;
            }();
        DefaultPlane->guid = GUID(2336475386456);
        DefaultPlane->name = "plane_mesh";


        DefaultQuad = [] {
            std::vector<Luxia::Rendering::Vertex> quad_verts{
            {{glm::vec3(-1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 0.0f)}},
            {{glm::vec3(1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 0.0f)}},
            {{glm::vec3(-1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 1.0f)}},
            {{glm::vec3(1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 1.0f)}}
            };
            std::vector<unsigned int> quad_inds{ 0, 1, 2,  1, 3, 2 };
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(
                quad_verts,
                quad_inds
            );
            mesh->CalculateMesh();
            return mesh;
            }();
        DefaultQuad->guid = GUID(4389563345345345);
        DefaultQuad->name = "Quad Mesh";


        // Materials
        ResourcesVector.push_back(NullShader);
        ResourcesVector.push_back(NullMaterial);
        ResourcesVector.push_back(DefaultLitShader);
        ResourcesVector.push_back(DefaultLitMaterial);
        ResourcesVector.push_back(DefaultUnlitShader);
        ResourcesVector.push_back(DefaultUnlitMaterial);
        ResourcesVector.push_back(DepthOnlyShader);
        ResourcesVector.push_back(DepthOnlyMaterial);
        ResourcesVector.push_back(DefaultSkyboxShader);
        ResourcesVector.push_back(DefaultSkyboxMaterial);
        // Meshes
        ResourcesVector.push_back(DefaultSphere);
        ResourcesVector.push_back(DefaultCube);
        ResourcesVector.push_back(DefaultPlane);
        ResourcesVector.push_back(DefaultQuad);
    }

    void ResourceManager::Cleanup() {
        ResourcesVector.clear();

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

        DefaultSkyboxMaterial->Unload();
        DefaultSkyboxShader->Unload();
        DefaultSkyboxShader->Delete();
        DefaultSkyboxMaterial.reset();
        DefaultSkyboxShader.reset();

        NullMaterial->Unload();
        NullShader->Unload();
        NullShader->Delete();
        DefaultUnlitMaterial.reset();
        DefaultUnlitShader.reset();
    }

}