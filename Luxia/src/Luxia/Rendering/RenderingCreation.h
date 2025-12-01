#pragma once

#include "Luxia/Core/Core.h"

#include "Luxia/Rendering/API/IRenderer.h"
#include "Luxia/Rendering/OpenGL/OpenGL_Renderer.h"

#include "Luxia/Rendering/API/IRenderBuffers.h"
#include "Luxia/Rendering/OpenGL/OpenGLRenderBuffers.h"

namespace Luxia::Platform::Rendering {

	inline std::shared_ptr<Luxia::Rendering::IRenderer> LUXIA_API CreateRenderer() {
		std::shared_ptr<Luxia::Rendering::IRenderer> renderer;
		#ifdef LUXIA_RENDERER_OPENGL
			renderer = std::make_shared<Luxia::Rendering::OpenGL::OpenGL_Renderer>();
		#else
			#error Luxia doesnt support your renderer!
		#endif // LUXIA_RENDERER_OPENGL
		return renderer;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::VAO> LUXIA_API CreateVAO() {
		std::shared_ptr<Luxia::Rendering::Buffers::VAO> vao;
		#ifdef LUXIA_RENDERER_OPENGL
			vao = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VAO>();
		#else
			#error Luxia doesnt support your renderer!		
		#endif // LUXIA_RENDERER_OPENGL

		return vao;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::VBO> LUXIA_API CreateVBO() {
		std::shared_ptr<Luxia::Rendering::Buffers::VBO> vbo;
		#ifdef LUXIA_RENDERER_OPENGL
			vbo = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VBO>();
		#else
			#error Luxia doesnt support your renderer!
		#endif // LUXIA_RENDERER_OPENGL

		return vbo;
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::EBO> LUXIA_API CreateEBO() {
		std::shared_ptr<Luxia::Rendering::Buffers::EBO> ebo;
		#ifdef LUXIA_RENDERER_OPENGL
			ebo = std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_EBO>();
		#else
			#error Luxia doesnt support your renderer!
		#endif // LUXIA_RENDERER_OPENGL

		return ebo;
	}
}