#pragma once

#include "Luxia/Core/Core.h"

#include "Luxia/Rendering/API/IRenderer.h"
#include "Luxia/Rendering/API/IRenderBuffers.h"


#ifdef LUXIA_RENDERER_OPENGL

#include "Luxia/Rendering/OpenGL/OpenGL_Renderer.h"
#include "Luxia/Rendering/OpenGL/OpenGLRenderBuffers.h"

namespace Luxia::Platform::Rendering {
	inline std::shared_ptr<Luxia::Rendering::IRenderer> LUXIA_API CreateRenderer() {
		return std::make_shared<Luxia::Rendering::OpenGL::OpenGL_Renderer>();
	}
	inline std::shared_ptr<Luxia::Rendering::Buffers::VAO> LUXIA_API CreateVAO() {
		return std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VAO>();
	}
	inline std::shared_ptr<Luxia::Rendering::Buffers::VBO> LUXIA_API CreateVBO() {
		return std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_VBO>();
	}

	inline std::shared_ptr<Luxia::Rendering::Buffers::EBO> LUXIA_API CreateEBO() {
		return std::make_shared<Luxia::Rendering::OpenGL::Buffers::OpenGL_EBO>();
	}
}

#elif LUXIA_RENDERER_VULKAN
#error Vulkan Renderer not yet supported in Luxia!
#endif

