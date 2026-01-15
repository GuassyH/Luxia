#include "lxpch.h"
#include "OpenGL_Renderer.h"
#include "Luxia/Managers/EventManager.h"
#include "Luxia/Rendering/OpenGL/OpenGL_UIRenderer.h"

#include "Luxia/Platform/AssetCreation.h"
#include "Luxia/Core/Log.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "Luxia/Managers/ResourceManager.h"

namespace Luxia::Rendering::OpenGL {

	OpenGL_Renderer::OpenGL_Renderer() {
		// Initialize GLFW
		if (!glfwInit()) { return; }

		// Set opengl version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_UIRenderer = std::make_shared<OpenGL_UIRenderer>();

		initialized = true;
	}

	/// Lights

	void OpenGL_Renderer::RecalculateLightBuffer(entt::registry& reg) {
		auto light_view = reg.view<Luxia::Components::Light>();
		
		std::vector<LightObject> lightObjects;

		numLights = 0;
		for (auto ent : light_view) {
			auto& light = reg.get<Luxia::Components::Light>(ent);
			if (light.transform) {

				LightObject newLO = LightObject();

				newLO.color = light.color;
				newLO.type = (int)light.lightType;
				newLO.position = light.transform->world_position;
				newLO.rotation = light.transform->GetRotVec(false);
				
				lightObjects.push_back(newLO);
				numLights++;
			}
		}

		lightBufferSize = sizeof(LightObject) * numLights;

		if (lightBufferID) glDeleteBuffers(1, &lightBufferID);
		glGenBuffers(1, &lightBufferID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, lightBufferSize, lightObjects.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightBufferID);
	}

	/// Basic render functions

	// Render the render object (meshrenderer mesh with material and transform)
	void OpenGL_Renderer::RenderRO(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) {
		RenderMesh(ro.mesh, ro.mat, ro.modelMat, viewMat, projMat);
	}

	void OpenGL_Renderer::RenderMesh(const Luxia::Mesh* m_mesh, Luxia::IMaterial* m_material, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!m_mesh)
			return;
		if (!m_mesh->IsValid())
			return;

		if (m_material) {
			if (m_material->shader) {
				m_material->Use(modMat, viewMat, projMat);
				// if is lit
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightBufferID);
				m_material->shader->SetInt("numLights", numLights);
			}
			else 
				ResourceManager::NullMaterial->Use(modMat, viewMat, projMat);
		}
		else
			ResourceManager::NullMaterial->Use(modMat, viewMat, projMat);

		m_mesh->vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh->indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh->vao->Unbind();
	}

	void OpenGL_Renderer::RenderMeshPure(const Mesh& m_mesh) {
		if (!m_mesh.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }

		m_mesh.vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh.vao->Unbind();
	}

	void OpenGL_Renderer::RenderFBO(const Mesh& m_quad, std::shared_ptr<IShader> fs_shader, std::shared_ptr<ITexture> cam_tex) {
		if (!m_quad.IsValid()) { LX_CORE_ERROR("Tried to render in-valid mesh"); return; }

		cam_tex->Bind();
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