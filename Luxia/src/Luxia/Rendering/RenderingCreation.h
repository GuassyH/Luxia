#pragma once

#include "Luxia/Core.h"

#include <iostream>

#include "Luxia/Rendering/API/IRenderer.h"
#include "Luxia/Rendering/OpenGL/OpenGL_Renderer.h"

#include "Luxia/Rendering/API/IRenderBuffers.h"
#include "Luxia/Rendering/OpenGL/OpenGLRenderBuffers.h"

namespace Luxia::Platform::Rendering {

	inline std::shared_ptr<Luxia::Rendering::IRenderer> CreateRenderer() {
		std::shared_ptr<Luxia::Rendering::IRenderer> renderer;
		#ifdef LUXIA_PLATFORM_WINDOWS
		renderer = std::make_shared<Luxia::Rendering::OpenGL::OpenGL_Renderer>();
		#endif // LUXIA_PLATFORM_WINDOWS
		return renderer;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::VAO> CreateVAO() {
		std::shared_ptr<Luxia::Rendering::Buffers::VAO> vao;
		#ifdef LUXIA_PLATFORM_WINDOWS
				vao = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VAO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return vao;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::VBO> CreateVBO() {
		std::shared_ptr<Luxia::Rendering::Buffers::VBO> vbo;
		#ifdef LUXIA_PLATFORM_WINDOWS
				vbo = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VBO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return vbo;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::EBO> CreateEBO() {
		std::shared_ptr<Luxia::Rendering::Buffers::EBO> ebo;
		#ifdef LUXIA_PLATFORM_WINDOWS
				ebo = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_EBO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return ebo;
	}
}