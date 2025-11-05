#pragma once

#include "Luxia/Core/Core.h"

namespace Luxia::Rendering::Buffers {

	class LUXIA_API VAO {
	public:
		VAO() = default;
		virtual ~VAO() = default;
		unsigned int ID = 0;
		virtual void LinkAttrib(unsigned int location, unsigned int numElements, unsigned int type, size_t stride, unsigned int offset) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Delete() = 0;
	};

	class LUXIA_API VBO {
	public:
		VBO() = default;
		virtual ~VBO() = default;
		unsigned int ID = 0;
		virtual void BindBufferData(size_t size, const void* data) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Delete() = 0;
	};

	class LUXIA_API EBO {
	public:
		EBO() = default;
		virtual ~EBO() = default;
		unsigned int ID = 0;
		virtual void BindBufferData(size_t size, const void* data) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Delete() = 0;
	};
}