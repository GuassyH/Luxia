#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Rendering/API/ICamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace Luxia::Platform::OpenGL {
	class GL_Camera : public ICamera {
	public:
		GL_Camera(int w, int h) : ICamera(w,h) {}
		~GL_Camera() = default;

		virtual void UpdateMatrix() override;
	};
}

