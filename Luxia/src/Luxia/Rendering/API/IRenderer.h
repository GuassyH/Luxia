#pragma once

#include "Luxia/Components/Transform.h"
#include "Luxia/Components/MeshRenderer.h"

#include "Luxia/Mesh.h"
#include "Luxia/Core/Core.h"

namespace Luxia::Rendering {
	struct RenderObject {
		Luxia::Components::Transform* transform;
		Luxia::Components::MeshRenderer* mesh_rend;
	};

	class LUXIA_API IRenderer {
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

		void Submit(const RenderObject ro) { renderObjects.push_back(ro); }
		void Flush(const glm::mat4& viewMat, const glm::mat4& projMat) { 
			for (auto& ro : renderObjects) { 
				RenderModel(ro, viewMat, projMat); 
			} 
			renderObjects.clear();
		}

		virtual void RenderModel(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
		virtual void RenderMesh(const Mesh& m_mesh, const std::shared_ptr<IShader> m_shader, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
	protected:
		std::vector<RenderObject> renderObjects;
	};
}