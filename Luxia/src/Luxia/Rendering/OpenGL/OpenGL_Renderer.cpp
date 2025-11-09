#include "lxpch.h"
#include "OpenGL_Renderer.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Rendering::OpenGL {

	void OpenGL_Renderer::RenderModel(const std::shared_ptr<IModel> m_model, const std::shared_ptr<IShader> m_shader, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!m_model || !m_shader) { LX_CORE_ERROR("Tried to render null meshrenderer!"); return; }
		for (auto& mesh : m_model->GetMeshes()) {
			RenderMesh(mesh, m_shader, modMat, viewMat, projMat);
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
}