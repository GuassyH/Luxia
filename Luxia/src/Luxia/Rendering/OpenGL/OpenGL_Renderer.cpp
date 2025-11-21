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
			// LX_CORE_ERROR("Tried to render null material!");
			return; }
		else if (!ro.transform){
			LX_CORE_ERROR("Tried to render null transform!");
			return; }


		ro.transform->UpdateMatrix();

		glm::mat4& modMat = ro.transform->GetMatrix();
		for (auto& mesh : ro.mesh_rend->model->GetMeshes()) {
			RenderMesh(mesh, ro.mesh_rend->material->shader, modMat, viewMat, projMat);
		}
	}

	void OpenGL_Renderer::RenderMesh(const Mesh& m_mesh, const std::shared_ptr<IShader> m_shader, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!m_mesh.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }

		m_shader->Use();

		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;
		unsigned int normalIdx = 0;

		for (unsigned int i = 0; i < m_mesh.textures.size(); i++) {
			// activate texture slot
			glActiveTexture(GL_TEXTURE0 + i); // Activate the texture unit first

			// retrieve texture number (the 0 in diffuse0)
			std::string name;
			switch (m_mesh.textures[i]->type) {
			case LX_TEXTURE_DIFFUSE:
				name = "diffuse" + std::to_string(diffuseIdx++);
				break;
			case LX_TEXTURE_SPECULAR:
				name = "specular" + std::to_string(specularIdx++);
				break;
			case LX_TEXTURE_NORMALS:
				name = "normals" + std::to_string(normalIdx++);
				break;
			}

			// Set shader texture value
			m_shader->SetInt(name.c_str(), i); // Set the sampler to the correct texture unit

			// Bind
			m_mesh.textures[i]->Use();
		}

		m_shader->SetBool("hasDiffuse", diffuseIdx > 0);
		m_shader->SetBool("hasSpecular", specularIdx > 0);
		m_shader->SetBool("hasNormals", normalIdx > 0);
		
		m_shader->SetMat4("modelMat", modMat);
		m_shader->SetMat4("viewMat", viewMat);
		m_shader->SetMat4("projMat", projMat);

		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh.vao->Unbind();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
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