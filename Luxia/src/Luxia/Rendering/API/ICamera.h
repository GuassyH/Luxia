#pragma once

#include "Luxia/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Luxia {
	class LUXIA_API ICamera {
	public:
		float FOVdeg = 70.0f;

		ICamera(int w, int h) : width(w), height(h) {}
		virtual ~ICamera() = default;

		virtual void UpdateMatrix() = 0;

		glm::mat4& GetViewMat() { return m_View; }
		glm::mat4& GetProjMat() { return m_Proj; }
		glm::vec2 GetRes() { return glm::vec2(width, height); }
	protected:
		int width = 10, height = 10;
		float nearPlane = 0.01f, farPlane = 1000.0f;

		glm::mat4 m_Proj = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);

		glm::vec3 Up = glm::vec3(0.0f);
		glm::vec3 Right = glm::vec3(0.0f);
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	};
}