#pragma once

#include "Luxia/Rendering/API/IRenderer.h"

namespace Luxia::Rendering::OpenGL {
	class OpenGL_Renderer : public Luxia::Rendering::IRenderer {
	public:
		OpenGL_Renderer() = default;
		~OpenGL_Renderer() = default;

		virtual void RenderModel(const std::shared_ptr<IModel> m_model, const std::shared_ptr<IShader> m_shader, const std::shared_ptr<ICamera> m_camera) override;
		virtual void RenderMesh(const Mesh& m_mesh, const std::shared_ptr<IShader> m_shader, const std::shared_ptr<ICamera> m_camera) override;
	};
}