#pragma once
#include "RenderingBuffers.h"


namespace Luxia::Rendering::OpenGL {

	class OpenGL_VAO : public Luxia::Rendering::VAO {
	public:
		OpenGL_VAO();
		~OpenGL_VAO() = default;
		unsigned int ID = 0;
		virtual void LinkAttrib(unsigned int location, unsigned int numElements, unsigned int type, size_t stride, unsigned int offset) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Delete() override;
	};

	class OpenGL_VBO : public Luxia::Rendering::VBO {
	public:
		OpenGL_VBO();
		~OpenGL_VBO() = default;
		unsigned int ID = 0;
		virtual void BindBufferData(size_t size, const void* data) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Delete() override;
	};

	class OpenGL_EBO : public Luxia::Rendering::EBO {
	public:
		OpenGL_EBO();
		~OpenGL_EBO() = default;
		unsigned int ID = 0;
		virtual void BindBufferData(size_t size, const void* data) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Delete() override;
	};
}