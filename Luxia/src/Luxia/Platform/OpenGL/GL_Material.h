#pragma once
#include "Luxia/Rendering/API/IMaterial.h"

namespace Luxia::Platform::OpenGL {
    class LUXIA_API GL_Material : public Luxia::IMaterial
    {
	public:
		GL_Material();
		GL_Material(const std::shared_ptr<IShader> matShader)
			: Luxia::IMaterial(matShader) {
		}
		~GL_Material() = default;

		virtual void Use(const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) override;
		virtual bool Unload() override;
    };
}
