#include "lxpch.h"
#include "OpenGL_Renderer.h"
#include "Luxia/Managers/EventManager.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Rendering::OpenGL {

	void OpenGL_Renderer::RenderModel(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!ro.mesh_rend->model) {
			LX_CORE_ERROR("Tried to render null model!");
			return;	}
		else if (!ro.mesh_rend->material || !ro.mesh_rend->material->shader){
			LX_CORE_ERROR("Tried to render null material!");
			return; }
		else if (!ro.transform){
			LX_CORE_ERROR("Tried to render null transform!");
			return; }


		ro.transform->UpdateMatrix();

		glm::mat4& modMat = ro.transform->GetMatrix();
		for (auto& mesh : ro.mesh_rend->model->GetMeshes()) {
			RenderMesh(mesh, modMat, viewMat, projMat);
		}
	}

	void OpenGL_Renderer::RenderMesh(const Mesh& m_mesh, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!m_mesh.IsValid()) { LX_CORE_ERROR("Tried to render invalid mesh"); return; }
		if (!m_mesh.material || !m_mesh.material->shader) {
			LX_CORE_ERROR("Tried to render mesh with invalid material or shader");
			return;
		}

		// Use Material
		m_mesh.material->Use();
		
		m_mesh.material->shader->Use();
		m_mesh.material->shader->SetMat4("modelMat", modMat);
		m_mesh.material->shader->SetMat4("viewMat", viewMat);
		m_mesh.material->shader->SetMat4("projMat", projMat);

		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh.vao->Unbind();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
	}

	void OpenGL_Renderer::RenderMeshPure(const Mesh& m_mesh) {
		if (!m_mesh.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }

		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh.vao->Unbind();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGL_Renderer::RenderFBO(const Mesh& m_quad, std::shared_ptr<IShader> fs_shader, std::shared_ptr<ITexture> cam_tex) {
		if (!m_quad.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }

		cam_tex->Use();
		fs_shader->Use();

		glActiveTexture(GL_TEXTURE0);
		fs_shader->SetInt("cam_texture", 0);
		fs_shader->SetVec2("window_size", Luxia::Screen::GetWindowSize());


		m_quad.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_quad.indices.size(), GL_UNSIGNED_INT, 0);

		m_quad.vao->Unbind();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}