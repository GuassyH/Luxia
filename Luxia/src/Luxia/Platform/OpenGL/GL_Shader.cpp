#include "GL_Shader.h"

namespace Luxia::Platform::OpenGL {
	
	void CheckCompileErrors(unsigned int shader, const std::string& type) {
		int success;
		char infoLog[1024];

		if (type != "PROGRAM") {
			// Shader compilation error checking
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				LX_CORE_ERROR("Shader Compilation Error (Type:{})", infoLog);
			}
		}
		else {
			// Shader program linking error checking
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				LX_CORE_ERROR("Program Linking Error\n{}", infoLog);
			}
		}
	}

	GL_Shader::GL_Shader(const char* frag_path, const char* vert_path) {
		std::string fragCode = ReadFile(frag_path);
		std::string vertCode = ReadFile(vert_path);

		const char* vertSource = vertCode.c_str();
		const char* fragSource = fragCode.c_str();

		Compile(fragSource, vertSource);
	}

	void GL_Shader::Compile(const char* fragSrc, const char* vertSrc) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertSrc, NULL);
		glCompileShader(vertexShader);
		CheckCompileErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragSrc, NULL);
		glCompileShader(fragmentShader);
		CheckCompileErrors(fragmentShader, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		CheckCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		LX_CORE_INFO("Succesfully Created shader!");
	}

	void GL_Shader::Use() {
		glUseProgram(ID);
	}

	void GL_Shader::Delete() {
		glDeleteProgram(ID);
	}

	void GL_Shader::SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value == true ? 1 : 0);
	}
	void GL_Shader::SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void GL_Shader::SetFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void GL_Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
	}
	void GL_Shader::SetVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void GL_Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
	}
	void GL_Shader::SetVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void GL_Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.a);
	}
	void GL_Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void GL_Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void GL_Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void GL_Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}