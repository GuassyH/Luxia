#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Platform/OpenGL/GL_Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace Luxia::Platform::OpenGL {
	class GL_Camera : public ICamera {
	public:
		GL_Camera(const int w, const int h) : ICamera(w, h) { 
			output_texture = std::make_shared<GL_Texture>(); 
			output_texture->CreateEmpty(w, h, 3);
		}
		~GL_Camera() = default;

		virtual std::shared_ptr<ITexture>& Render() override { return output_texture; }
		virtual void UpdateMatrix() override;
	};
}

