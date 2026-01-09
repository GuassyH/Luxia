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
		#ifdef LUXIA_RENDERER_OPENGL
			glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		#endif // LUXIA_RENDERER_OPENGL
	}

	inline glm::vec2 GetMousePosRect(const glm::vec2 rect_pos, const glm::vec2 rect_size, const glm::vec2 wanted_size, const glm::vec2 mouse_pos) {
		glm::vec2 result = glm::vec2(0);

		// [0, 1]
		result = mouse_pos - rect_pos;
		result /= rect_size;
		result.y = 1.0f - result.y;

		// [0, Wanted]
		result *= wanted_size;

		// Clamp
		result = glm::clamp(result, glm::vec2(0.0f), wanted_size);

		return result;
	}

	// Returns between 0 & 1 on x & y
	inline glm::vec2 GetMousePosRectClamped(const glm::vec2 rect_pos, const glm::vec2 rect_size, const glm::vec2 mouse_pos) {
		glm::vec2 result = glm::vec2(0);

		result = mouse_pos - rect_pos;
		result /= rect_size;
		result = glm::clamp(result, 0.0f, 1.0f);
		result.y = 1.0f - result.y;

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