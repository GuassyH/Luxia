#pragma once

#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Core/Log.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Shader : public IShader {
	public:
		GL_Shader() = default;
		GL_Shader(const char* frag_path, const char* vert_path);
		~GL_Shader() = default;

		virtual void Compile(const char* fragSrc, const char* vertSrc) override;
		virtual void Use() override;
		virtual void Delete() override;

		virtual void SetBool(const std::string& name, bool value) const override;
		virtual void SetInt(const std::string& name, int value) const override;
		virtual void SetFloat(const std::string& name, float value) const override;
		virtual void SetVec2(const std::string& name, const glm::vec2& value) const override;
		virtual void SetVec2(const std::string& name, float x, float y) const override;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) const override;
		virtual void SetVec3(const std::string& name, float x, float y, float z) const override;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) const override;
		virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const override;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat) const override;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const override;
	};
}

