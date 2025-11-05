#include "OpenGL_Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Luxia::Rendering::OpenGL {

	void OpenGL_Renderer::RenderModel(const std::shared_ptr<IModel> m_model, const std::shared_ptr<IShader> m_shader, const std::shared_ptr<ICamera> m_camera) {
		for (auto& mesh : m_model->GetMeshes()) {
			RenderMesh(mesh, m_shader, m_camera);
		}
	}
	void OpenGL_Renderer::RenderMesh(const Mesh& m_mesh, const std::shared_ptr<IShader> m_shader, const std::shared_ptr<ICamera> m_camera) {
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

		glm::mat4 modelmat = glm::mat4(1.0f);
		modelmat = glm::translate(modelmat, glm::vec3(0.0f, -1.0f, -7.0f));
		modelmat = glm::scale (modelmat, glm::vec3(0.02f));

		glm::quat rotY = glm::angleAxis(glm::radians((float)glfwGetTime() * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotX = glm::angleAxis(glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat rotZ = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationQuat = rotY * rotX * rotZ; // Apply Z rotation first, then X, then Y

		glm::mat4 rotationMat = glm::mat4_cast(rotationQuat);
		modelmat *= rotationMat;
		m_shader->SetMat4("modelMat", modelmat);
		m_shader->SetMat4("viewMat", m_camera->GetViewMat());
		m_shader->SetMat4("projMat", m_camera->GetProjMat());

		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh.vao->Unbind();
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}