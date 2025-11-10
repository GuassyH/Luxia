#include "lxpch.h"
#include "GL_Camera.h"
#include "Luxia/Components/Transform.h"

namespace Luxia::Platform::OpenGL {

	void GL_Camera::UpdateMatrix(const glm::vec3& pos, const glm::vec3& rot) {
		// Initialise the matrices
		m_View = glm::lookAt(pos, pos + rot, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

		Forward = glm::normalize(rot);
		Right = glm::normalize(glm::cross(Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		Up = glm::normalize(glm::cross(Right, Forward));
	}

	void GL_Camera::Render(std::shared_ptr<Luxia::Scene> scene, std::shared_ptr<Luxia::Rendering::IRenderer> rend) {
		auto view = scene->GetEntitiesWith<Luxia::Components::MeshRenderer>();

		for (auto entity : view) {
			auto& mr = view.get<Luxia::Components::MeshRenderer>(entity);
			auto mr_t = scene->TryGetFromEntity<Luxia::Components::Transform>(entity);

			if (mr_t) {
				Luxia::Rendering::RenderObject ro { mr_t, &mr };
				rend->Submit(ro);
			}
			else {
				LX_CORE_ERROR("MeshRenderer ({}) has no transform!", (int)entity);
			}
		}

		rend->Flush(GetViewMat(), GetProjMat());
	}
}