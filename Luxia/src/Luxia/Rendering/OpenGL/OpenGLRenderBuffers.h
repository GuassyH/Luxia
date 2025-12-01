#pragma once
#include "Luxia/Rendering/API/IRenderBuffers.h"


namespace Luxia::Rendering::OpenGL::Buffers {

	class LUXIA_API OpenGL_VAO : public Luxia::Rendering::Buffers::VAO {
	public:
		OpenGL_VAO();
		~OpenGL_VAO() = default;
		unsigned int ID = 0;
		virtual void LinkAttrib(unsigned int location, unsigned int numElements, unsigned int type, size_t stride, unsigned int offset) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Delete() override;
	};

	class LUXIA_API OpenGL_VBO : public Luxia::Rendering::Buffers::VBO {
	public:
		OpenGL_VBO();
		~OpenGL_VBO() = default;
		unsigned int ID = 0;
		virtual void BindBufferData(size_t size, const void* data) override;
		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Delete() override;
	};

	class LUXIA_API OpenGL_EBO : public Luxia::Rendering::Buffers::EBO {
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