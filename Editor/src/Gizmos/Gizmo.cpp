#include "Gizmo.h"
#include "EditorPanels/SceneViewport.h"

namespace Editor::Gizmos {

	std::shared_ptr<Luxia::IMaterial> GizmoResources::xMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::yMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::zMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::orgioMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::hoverMaterial = nullptr;

	std::shared_ptr<Luxia::Mesh> GizmoResources::arrowMesh = nullptr;
	std::shared_ptr<Luxia::Mesh> GizmoResources::scaleMesh = nullptr;
	std::shared_ptr<Luxia::Mesh> GizmoResources::rotateQMesh = nullptr;
	std::shared_ptr<Luxia::Mesh> GizmoResources::rotateWMesh = nullptr;
	std::shared_ptr<Luxia::Mesh> GizmoResources::biAxisPlane = nullptr;

	void GizmoResources::Init(std::filesystem::path path_to_gizmos) {
		// Materials
		xMaterial = Luxia::Platform::Assets::CreateMaterial();
		yMaterial = Luxia::Platform::Assets::CreateMaterial();
		zMaterial = Luxia::Platform::Assets::CreateMaterial();
		orgioMaterial = Luxia::Platform::Assets::CreateMaterial();
		hoverMaterial = Luxia::Platform::Assets::CreateMaterial();

		xMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		xMaterial->color = glm::vec4(1.0f, 0.4392f, 0.4784f, 1.0f);

		yMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		yMaterial->color = glm::vec4(0.5569f, 1.0f, 0.5569f, 1.0f);

		zMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		zMaterial->color = glm::vec4(0.1765f, 0.9020f, 1.0f, 1.0f);

		orgioMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		orgioMaterial->color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

		hoverMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		hoverMaterial->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		path_to_gizmos = path_to_gizmos.lexically_normal();
		// Meshes
		std::shared_ptr<Luxia::IModel> arrowModel = Luxia::Platform::Assets::CreateModel();
		arrowModel->LoadFromPath(path_to_gizmos / "translate/arrow_cubed.fbx");
		arrowMesh = arrowModel->meshes[0];

		std::shared_ptr<Luxia::IModel> scaleModel = Luxia::Platform::Assets::CreateModel();
		scaleModel->LoadFromPath(path_to_gizmos / "scale/scale.fbx");
		scaleMesh = scaleModel->meshes[0];

		std::shared_ptr<Luxia::IModel> rotateModel = Luxia::Platform::Assets::CreateModel();
		rotateModel->LoadFromPath(path_to_gizmos / "rotate/rotate.fbx");
		rotateQMesh = rotateModel->meshes[0];
		rotateWMesh = rotateModel->meshes[1];

		std::shared_ptr<Luxia::IModel> biAxisPlaneModel = Luxia::Platform::Assets::CreateModel();
		biAxisPlaneModel->LoadFromPath(path_to_gizmos / "biaxis/plane.fbx");
		biAxisPlane = biAxisPlaneModel->meshes[0];
	
	}

}
