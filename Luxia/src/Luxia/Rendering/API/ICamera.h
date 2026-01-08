#pragma once

#include "Luxia/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Rendering/API/IRenderer.h"

#include "Luxia/Components/MeshRenderer.h"


namespace Luxia {
	class Scene;
	class LUXIA_API ICamera {
	public:
		float FOVdeg = 70.0f;

		ICamera() = default;
		virtual ~ICamera() = default;

		virtual std::shared_ptr<ITexture> Render(const std::shared_ptr<Luxia::Scene> scene, const std::shared_ptr<Luxia::Rendering::IRenderer> rend, const int width, const int height, const glm::vec4& clear_col) = 0;
		virtual void UpdateMatrix(const glm::vec3& pos, const glm::vec3& rot, const float FOVdeg, const int width, const int height, const float nearPlane, const float farPlane) = 0;

		const glm::mat4& GetViewMat() const { return m_View; }
		const glm::mat4& GetProjMat() const { return m_Proj; }

		const glm::vec3 GetForward() const { return Forward; }
		const glm::vec3 GetUp() const { return Up; }
		const glm::vec3 GetRight() const { return Right; }
	protected:
		std::shared_ptr<ITexture> output_texture;

		glm::mat4 m_Proj = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);

		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
	};
}