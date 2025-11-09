#pragma once

#include "Luxia/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Rendering/API/IRenderer.h"

#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Scene.h"

namespace Luxia {
	class LUXIA_API ICamera {
	public:
		float FOVdeg = 70.0f;

		ICamera(const int w, const int h) : width(w), height(h) {}
		virtual ~ICamera() = default;

		virtual void Render(std::shared_ptr<Luxia::Scene> scene, std::shared_ptr<Luxia::Rendering::IRenderer> rend) = 0;
		virtual void UpdateMatrix(const glm::vec3& pos, const glm::vec3& rot) = 0;

		glm::mat4& GetViewMat() { return m_View; }
		glm::mat4& GetProjMat() { return m_Proj; }
		glm::vec2 GetRes() { return glm::vec2(width, height); }
		void SetRes(const int w, const int h) { width = w; height = h; }
	protected:
		std::shared_ptr<ITexture> output_texture;

		int width = 10, height = 10;
		float nearPlane = 0.01f, farPlane = 1000.0f;

		glm::mat4 m_Proj = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);

		glm::vec3 Up = glm::vec3(0.0f);
		glm::vec3 Right = glm::vec3(0.0f);
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	};
}