#include "lxpch.h"
#include "OpenGLRenderBuffers.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Rendering::OpenGL::Buffers {
	// ======== VAO ========

	OpenGL_VAO::OpenGL_VAO() { glGenVertexArrays(1, &ID); }
	void OpenGL_VAO::Bind() { glBindVertexArray(ID); }
	void OpenGL_VAO::Unbind() { glBindVertexArray(0); }
	void OpenGL_VAO::Delete() { glDeleteVertexArrays(1, &ID); }

	void OpenGL_VAO::LinkAttrib(unsigned int location, unsigned int numElements, unsigned int type, size_t stride, unsigned int offset) {
		OpenGL_VAO::Bind();
		glVertexAttribPointer(location, numElements, type, GL_FALSE, stride, (void*)(offset));
		glEnableVertexAttribArray(location);
	}

	// ======== VBO ========

	OpenGL_VBO::OpenGL_VBO() { glGenBuffers(1, &ID); }
	void OpenGL_VBO::Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void OpenGL_VBO::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	void OpenGL_VBO::Delete() { glDeleteBuffers(1, &ID); }

	void OpenGL_VBO::BindBufferData(size_t size, const void* data) {
		OpenGL_VBO::Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	// ======== EBO ========

	OpenGL_EBO::OpenGL_EBO() { glGenBuffers(1, &ID); }
	void OpenGL_EBO::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	void OpenGL_EBO::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	void OpenGL_EBO::Delete() { glDeleteBuffers(1, &ID); }

	void OpenGL_EBO::BindBufferData(size_t size, const void* data) {
		OpenGL_EBO::Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
}