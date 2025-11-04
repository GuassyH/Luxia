#include "GL_Camera.h"
#include "glm/common.hpp"
#include "Luxia/Log.h"
namespace Luxia::Platform::OpenGL {

	void GL_Camera::UpdateMatrix() {
		// Initialise the matrices
		
		/// THIS IS WRONG!!! I forgot that rotation is in degrees not a vector, it works but i should change
		m_View = glm::lookAt(Position, Position + Orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

		//LX_CORE_INFO("{}deg {}x{}", FOVdeg, nearPlane, farPlane);

		Forward = glm::normalize(Orientation);
		Right = glm::normalize(glm::cross(Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		Up = glm::normalize(glm::cross(Right, Forward));
	}

}