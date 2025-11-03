#pragma once

#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Mesh.h"

namespace Luxia::Rendering {
	class IRenderer {
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		virtual void RenderModel(const std::shared_ptr<IModel> m_model) = 0;
		virtual void RenderMesh(const Mesh& m_mesh) = 0;
	};
}