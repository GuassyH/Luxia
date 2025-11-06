#pragma once

#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Mesh.h"
#include "Luxia/Core/Core.h"

namespace Luxia::Rendering {
	class LUXIA_API IRenderer {
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual void RenderModel(const std::shared_ptr<IModel> m_model, const std::shared_ptr<IShader> m_shader, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
		virtual void RenderMesh(const Mesh& m_mesh, const std::shared_ptr<IShader> m_shader, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
	};
}