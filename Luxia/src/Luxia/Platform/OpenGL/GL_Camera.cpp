#include "lxpch.h"
#include "GL_Camera.h"


namespace Luxia::Platform::OpenGL {

	void GL_Camera::UpdateMatrix() {
		// Initialise the matrices
		
		m_View = glm::lookAt(Position, Position + Orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

		Forward = glm::normalize(Orientation);
		Right = glm::normalize(glm::cross(Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		Up = glm::normalize(glm::cross(Right, Forward));
	}

	void GL_Camera::Render(entt::registry& reg, std::shared_ptr<Luxia::Rendering::IRenderer> rend) {
		auto view = reg.view<Luxia::Components::MeshRenderer>();

		for (auto entity : view) {
			auto& mr = view.get<Luxia::Components::MeshRenderer>(entity);
			rend->RenderModel(mr.model, mr.shader, GetViewMat(), GetProjMat());
		}
	}
}