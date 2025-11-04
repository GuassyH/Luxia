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
		glm::mat4 modelmat = glm::mat4(1.0f);
		modelmat = glm::translate(modelmat, glm::vec3(0.0f, 0.0f, -10.0f));
		//modelmat = glm::scale(modelmat, glm::vec3(0.1f));
		glm::quat rotY = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat rotZ = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat rotationQuat = rotY * rotX * rotZ; // Apply Z rotation first, then X, then Y

		glm::mat4 rotationMat = glm::mat4_cast(rotationQuat);
		modelmat *= rotationMat;
		glUniformMatrix4fv(glGetUniformLocation(m_shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelmat));
		glUniformMatrix4fv(glGetUniformLocation(m_shader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(m_camera->GetViewMat()));
		glUniformMatrix4fv(glGetUniformLocation(m_shader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(m_camera->GetProjMat()));
		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);
		m_mesh.vao->Unbind();
	}
}