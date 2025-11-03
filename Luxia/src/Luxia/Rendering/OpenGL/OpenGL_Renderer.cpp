#include "OpenGL_Renderer.h"

namespace Luxia::Rendering::OpenGL {

	void OpenGL_Renderer::RenderModel(const std::shared_ptr<IModel> m_model) {
		for (auto& mesh : m_model->GetMeshes()) {
			mesh.vao->Bind();
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
	}
	void OpenGL_Renderer::RenderMesh(const Mesh& m_mesh) {
		if (!m_mesh.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }
		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);
	}
}