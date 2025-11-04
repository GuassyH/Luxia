#pragma once

#include "Luxia/Core.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <glm/common.hpp>
#include <glm/glm.hpp>

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

	class LUXIA_API IShader {
	public:
		unsigned int ID;

		IShader() = default;
		IShader(const char* frag_path, const char* vert_path);
		virtual ~IShader() = default;

		virtual void Compile(const char* fragSrc, const char* vertSrc) = 0;
		virtual void Use() = 0;
		virtual void Delete() = 0;

		void SetBool(const std::string& name, bool value) const {}						// should implement
		void SetInt(const std::string& name, int value) const {}						// should implement
		void SetFloat(const std::string& name, float value) const {}					// should implement
		void SetVec2(const std::string& name, const glm::vec2& value) const {}			// should implement
		void SetVec2(const std::string& name, float x, float y) const {}				// should implement
		void SetVec3(const std::string& name, const glm::vec3& value) const {}			// should implement
		void SetVec3(const std::string& name, float x, float y, float z) const {}		// should implement
		void SetVec4(const std::string& name, const glm::vec4& value) const {}			// should implement
		void SetVec4(const std::string& name, float x, float y, float z, float w) {}	// should implement
		void SetMat2(const std::string& name, const glm::mat2& mat) const {}			// should implement
		void SetMat3(const std::string& name, const glm::mat3& mat) const {}			// should implement
		void SetMat4(const std::string& name, const glm::mat4& mat) const {}			// should implement
	protected:
		std::string fragment_path;
		std::string vertex_path;
	};
}