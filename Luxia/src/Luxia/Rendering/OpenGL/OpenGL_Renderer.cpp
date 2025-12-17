#include "lxpch.h"
#include "OpenGL_Renderer.h"
#include "Luxia/Managers/EventManager.h"
#include "Luxia/Rendering/OpenGL/OpenGL_UIRenderer.h"

#include "Luxia/Platform/AssetCreation.h"
#include "Luxia/Core/Log.h"

#include "glfw/glfw3.h"
#include <KHR/khrplatform.h>
#include "glad/glad.h"

namespace Luxia::Rendering::OpenGL {

	OpenGL_Renderer::OpenGL_Renderer() {
		m_UIRenderer = std::make_shared<OpenGL_UIRenderer>();

		// Should be baked into the engine somehow
		null_shader = Platform::Assets::CreateShader("E:/BuiltLuxia/Sandbox/assets/shaders/default.frag", "E:/BuiltLuxia/Sandbox/assets/shaders/default.vert");
		null_mat = Platform::Assets::CreateMaterial(null_shader);
		null_mat->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	
		default_shader = Platform::Assets::CreateShader("E:/BuiltLuxia/Sandbox/assets/shaders/default.frag", "E:/BuiltLuxia/Sandbox/assets/shaders/default.vert");
		default_mat = Platform::Assets::CreateMaterial(default_shader);
	}

	// Render the render object (meshrenderer mesh with material and transform)
	void OpenGL_Renderer::RenderRO(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) {
		RenderMesh(ro.mesh_rend->mesh, ro.mesh_rend->material, ro.transform->GetMatrix(), viewMat, projMat);
	}

	void OpenGL_Renderer::RenderMesh(const std::shared_ptr<Luxia::Mesh> m_mesh, const std::shared_ptr<Luxia::IMaterial> m_material, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (!m_mesh) 
			return;
		if (!m_mesh->IsValid())
			return;
		

		bool valid_mat = false;

		if (m_material)
			if (m_material->shader)
				valid_mat = true;
		
		if(valid_mat)
			m_material->Use(modMat, viewMat, projMat);
		else if (null_mat)
			null_mat->Use(modMat, viewMat, projMat);
	

		m_mesh->vao->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh->indices.size(), GL_UNSIGNED_INT, 0);

		m_mesh->vao->Unbind();
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