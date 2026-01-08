#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IWindow.h"
#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include "Luxia/Rendering/API/IRenderer.h"

#include "Luxia/Components/Camera.h"
#include "Luxia/Managers/ResourceManager.h"
#include "Luxia/Scene/Scene.h"

#include <glm/glm.hpp>
#include <xmemory>

namespace Luxia::Screen {
	
	inline void BindFBO(int FBO) {
		#ifdef LUXIA_RENDERER_OPENGL
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		#endif // LUXIA_RENDERER_OPENGL
	}
	inline void BindRBO(int RBO) {
	}

	inline void ClearFBOTex(std::shared_ptr<Luxia::ITexture> tex, const int width, const int height, const glm::vec4& clear_col) {
		#ifdef LUXIA_RENDERER_OPENGL
			if (width != tex->GetWidth() || height != tex->GetHeight()) {
				tex->Delete();
				tex->CreateFBOTex(width, height);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, tex->GetFBO());
			glClearColor(clear_col.r, clear_col.g, clear_col.b, clear_col.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, width, height);
		#endif // LUXIA_RENDERER_OPENGL
	};

	// Returns between 0 & 1 on x & y
	inline glm::vec2 GetMousePosRect(const glm::vec2 rect_pos, const glm::vec2 rect_size, const glm::vec2 mouse_pos) {
		glm::vec2 result = glm::vec2(0);

		result = mouse_pos - rect_pos;
		result /= rect_size;
		result = glm::clamp(result, 0.0f, 1.0f);
		result.y = 1.0f - result.y;

		return result;
	}

	inline Luxia::GUID GetMousePosEntity(glm::vec2 mouse_pos,
		Luxia::Components::Camera* cam,
		const std::shared_ptr<Luxia::Scene> scene,
		const std::shared_ptr<Luxia::Rendering::IRenderer> renderer,
		std::shared_ptr<Luxia::ITexture> output_texture) {
		
		if (!output_texture->IsValid() || !output_texture->IsFBOTex()) return Luxia::GUID(0);
		
		Luxia::GUID result = GUID(0);

		ClearFBOTex(output_texture, cam->width, cam->height, glm::vec4(1.0f));

		// Creates a copy of the unlit mat (so we can change color no problem)
		std::shared_ptr<Luxia::IMaterial> id_mat = Platform::Assets::CreateMaterial();
		id_mat->shader = ResourceManager::DefaultUnlitMaterial->shader;
		glm::vec4 id_color = glm::vec4(0.0f);

		auto mesh_view = scene->GetEntitiesWith<Luxia::Components::MeshRenderer>();
		// cam->GetCamera()->UpdateMatrix(cam->transform->position, cam->transform->GetRotVec(), cam->FOVdeg, cam->width, cam->height, cam->nearPlane, cam->farPlane);

		for (auto entity : mesh_view) {
			auto& mr = scene->GetFromEntity<Luxia::Components::MeshRenderer>(entity);

			if (mr.transform) {
				int id = (int)mr.transform->ent_id;
				id_color.r = ((id & 0x000000FF) >> 0) / 255.0f;
				id_color.g = ((id & 0x0000FF00) >> 8) / 255.0f;
				id_color.b = ((id & 0x00FF0000) >> 16) / 255.0f;
				id_color.a = 1.0f;
				id_mat->color = id_color;

				renderer->RenderMesh(mr.mesh.get(), id_mat.get(), mr.transform->GetMatrix(),
					cam->GetCamera()->GetViewMat(), cam->GetCamera()->GetProjMat());
			}
		}
		// temp, since uses GL
		GLubyte pixel[4];
		glReadPixels(static_cast<GLint>(cam->width * mouse_pos.x), static_cast<GLint>(cam->height * mouse_pos.y), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel);

		// if pixel is white (255, 255, 255) then its the LAST ID aka no entity (unless you have 16777215 (255^3) entities) 
		if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
			// Do sum?
			LX_ERROR("NO PICKED ENTITY");
		}
		else {
			unsigned int pickedID = (static_cast<unsigned int>(pixel[0]) << 0) | (static_cast<unsigned int>(pixel[1]) << 8) | (static_cast<unsigned int>(pixel[2]) << 16);
			result = scene->GetFromEntity<Luxia::Components::Transform>(entt::entity(pickedID)).ent_guid;
			LX_INFO("PICKED ENTITY: {}", (uint64_t)result);
		}

		BindFBO(0);
		BindRBO(0);
		output_texture->Unbind();

		return result;
	}

	// Returns the ray direction for a mouse position [0, 1] for a given camera
	inline glm::vec3 GetRayDir(glm::vec2 mouse_pos, const Luxia::Components::Camera* cam) {
		// NDC [-1, 1]
		glm::vec2 NDC = (mouse_pos * 2.0f) - glm::vec2(1.0f);

		float fov = glm::radians(cam->FOVdeg);
		float aspect = (float)cam->width / (float)cam->height;
		float scale = tan(fov * 0.5f);

		const Luxia::ICamera* pure_cam = cam->GetCamera();
		
		glm::vec3 rayDir = glm::normalize(pure_cam->GetForward() + NDC.x * aspect * scale * pure_cam->GetRight() + NDC.y * scale * pure_cam->GetUp());

		return rayDir;
	}

}