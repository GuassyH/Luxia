#pragma once

#include "Luxia/Core/Core.h"

#include "Luxia/Asset/Asset.h"
#include <glm/glm.hpp>
#include <fstream>

namespace Luxia {

	// READFILE IS BORROWED
	inline std::string ReadFile(const std::string& filepath) {
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filepath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	class LUXIA_API IShader : public Assets::Asset {
	public:
		unsigned int ID = 0;

		IShader() { Luxia::AssetType::Shader; }
		IShader(const char* frag_path, const char* vert_path) { }
		virtual ~IShader() = default;

		virtual void Compile(const char* fragSrc, const char* vertSrc) = 0;
		virtual void Use() = 0;
		virtual void Delete() = 0;
		virtual bool Unload() override = 0;

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetVec2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetVec2(const std::string& name, float x, float y) const = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetVec3(const std::string& name, float x, float y, float z) const = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const = 0;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat) const = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) const = 0;
	
		std::string GetFragPath() const { return fragment_path; }
		std::string GetVertPath() const { return vertex_path; }
	protected:
		std::string fragment_path;
		std::string vertex_path;
	};
}