#pragma once

#include "Luxia/Rendering/API/IRenderer.h"

namespace Luxia::Rendering::OpenGL {
	class LUXIA_API OpenGL_Renderer : public Luxia::Rendering::IRenderer {
	public:
		OpenGL_Renderer();
		~OpenGL_Renderer() = default;

		// Step based

		// Basic render functions
		virtual void RenderRO(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) override;
		virtual void RenderMesh(const Luxia::Mesh* m_mesh, Luxia::IMaterial* m_material, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) override;
		virtual void RenderMeshPure(const Mesh& m_mesh) override;
		virtual void RenderFBO(const Mesh& m_quad, std::shared_ptr<IShader> fs_shader, std::shared_ptr<ITexture> cam_tex) override;
	};
}