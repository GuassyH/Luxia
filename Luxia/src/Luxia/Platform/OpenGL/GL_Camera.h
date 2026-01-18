#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Platform/OpenGL/GL_Texture.h"


namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Camera : public ICamera {
	public:
		GL_Camera(int w, int h) { 
			output_texture = std::make_shared<GL_Texture>(); 
			output_texture->CreateFBOTex(w, h);
		}
		~GL_Camera() = default;

		virtual std::shared_ptr<ITexture> Render(const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> rend, const bool useSkybox, const int width, const int height, const glm::vec4& clear_col) override;
		virtual void UpdateMatrix(const glm::vec3& pos, const Luxia::Components::Transform* transform, const float fovdeg, const int width, const int height, const float nearPlane, const float farPlane) override;
	};
}

