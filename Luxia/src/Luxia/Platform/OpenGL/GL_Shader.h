#pragma once

#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Log.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform::OpenGL {
	class GL_Shader : public IShader {
	public:
		GL_Shader() = default;
		GL_Shader(const char* frag_path, const char* vert_path);
		~GL_Shader() = default;

		virtual void Compile(const char* fragSrc, const char* vertSrc) override;
		virtual void Use() override;
		virtual void Delete() override;
	};
}

