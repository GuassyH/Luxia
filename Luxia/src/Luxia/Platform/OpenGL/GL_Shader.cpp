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

}