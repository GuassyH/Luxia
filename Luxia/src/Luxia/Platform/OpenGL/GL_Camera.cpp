#include "lxpch.h"
#include "GL_Camera.h"
#include "Luxia/Components/Transform.h"

#include "Luxia/Tools/Screen.h"
#include "Luxia/Scene/Scene.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Luxia::Platform::OpenGL {

	// Only update on change?
	void GL_Camera::UpdateMatrix(const glm::vec3& pos, const glm::vec3& rot, const float fovdeg, const int width, const int height, const float nearPlane, const float farPlane) {
		// Initialise the matrices
		m_View = glm::lookAt(pos, pos + rot, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Proj = glm::perspective(glm::radians(fovdeg), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);
		FOVdeg = fovdeg;

		// Skips recalculating the other vectors unecessarily 
		glm::vec3 f = glm::normalize(rot);
		if (Forward != f) {
			Forward = f;
			Right = glm::normalize(glm::cross(Forward, glm::vec3(0.0f, 1.0f, 0.0f)));
			Up = glm::normalize(glm::cross(Right, Forward));
		}
	}
	
	// Feels wierd? Why even have abstract camera now
	std::shared_ptr<ITexture> GL_Camera::Render(const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> rend, const bool useSkybox, const int width, const int height, const glm::vec4& clear_col) {
		if (width != output_texture->GetWidth() || height != output_texture->GetHeight()) {
			output_texture->Unload();
			output_texture->CreateFBOTex(width, height);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, output_texture->GetFBO());
		glBindRenderbuffer(GL_RENDERBUFFER, output_texture->GetRBO());
		glClearColor(clear_col.r, clear_col.g, clear_col.b, clear_col.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		if (useSkybox) {
			// Do skybox stuff
			auto mc = scene->GetMainLight();
			auto skybox = Luxia::ResourceManager::DefaultSkyboxMaterial;

			skybox->Use();
			skybox->shader->SetInt("screenWidth", width);
			skybox->shader->SetInt("screenHeight", height);
			skybox->shader->SetFloat("FOVdeg", FOVdeg);
				
			if (mc) {
				skybox->shader->SetVec3("sunDir", mc->transform->GetRotVec());
				skybox->shader->SetVec4("sunColor", mc->color);
			}
			else {
				skybox->shader->SetVec3("sunDir", glm::vec3(0.0f, 0.0f, -1.0f));
				skybox->shader->SetVec4("sunColor", glm::vec4(0.0f));
			}

			skybox->shader->SetVec3("camForward", GetForward());
			skybox->shader->SetVec3("camRight", GetRight());
			skybox->shader->SetVec3("camUp", GetUp());

			rend->RenderMeshPure(*Luxia::ResourceManager::DefaultQuad);
				
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		rend->Flush(GetViewMat(), GetProjMat());

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return output_texture;
	}

}