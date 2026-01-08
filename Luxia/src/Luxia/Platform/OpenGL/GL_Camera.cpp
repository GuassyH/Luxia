#include "lxpch.h"
#include "GL_Camera.h"
#include "Luxia/Components/Transform.h"

#include "Luxia/Tools/Screen.h"
#include "Luxia/Scene/Scene.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform::OpenGL {

	// Only update on change?
	void GL_Camera::UpdateMatrix(const glm::vec3& pos, const glm::vec3& rot, const float FOVdeg, const int width, const int height, const float nearPlane, const float farPlane) {
		// Initialise the matrices
		m_View = glm::lookAt(pos, pos + rot, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

		// Skips recalculating the other vectors unecessarily 
		glm::vec3 f = glm::normalize(rot);
		if (Forward != f) {
			Forward = f;
			Right = glm::normalize(glm::cross(Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
			Up = glm::normalize(glm::cross(Right, Forward));
		}
	}
	
	std::shared_ptr<ITexture> GL_Camera::Render(const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> rend, const int width, const int height, const glm::vec4& clear_col) {
		Luxia::Screen::ClearFBOTex(output_texture, width,height,clear_col);

		rend->Flush(GetViewMat(), GetProjMat());

		Luxia::Screen::BindFBO(0);

		return output_texture;
	}

}