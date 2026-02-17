#include "Gizmo.h"

namespace Editor::Gizmos {

	std::shared_ptr<Luxia::IMaterial> GizmoResources::xMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::yMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::zMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::orgioMaterial = nullptr;
	std::shared_ptr<Luxia::IMaterial> GizmoResources::hoverMaterial = nullptr;

	void GizmoResources::Init() {
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

		zMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;

		hoverMaterial->shader = Luxia::ResourceManager::DefaultUnlitShader;
		hoverMaterial->color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}


	/// Gizmo Part

	// Translate
	bool ArrowGizmoPart::OnClick() {
		return false;
	}
	glm::vec3 ArrowGizmoPart::OnDrag() {
		return glm::vec3(0.0f);
	}

	// Scale
	bool ScaleGizmoPart::OnClick() {
		return false;
	}
	glm::vec3 ScaleGizmoPart::OnDrag() {
		return glm::vec3(0.0f);
	}

	// Rotate
	bool RotateGizmoPart::OnClick() {
		return false;
	}
	glm::vec3 RotateGizmoPart::OnDrag() {
		return glm::vec3(0.0f);
	}
}
