#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Platform/OpenGL/GL_Texture.h"


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

