#pragma once

#include "Luxia/Rendering/API/IRenderer.h"
#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Rendering::OpenGL {
	class OpenGL_Renderer : public Luxia::Rendering::IRenderer {
	public:
		OpenGL_Renderer() = default;
		~OpenGL_Renderer() = default;

		virtual void RenderModel(const std::shared_ptr<IModel> m_model) override;
		virtual void RenderMesh(const Mesh& m_mesh) override;
	};
}