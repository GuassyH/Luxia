#pragma once

#include "Luxia/Components/Transform.h"
#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include "Luxia/Rendering/API/IUIRenderer.h"

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
				RenderRO(ro, viewMat, projMat); 
			} 
			renderObjects.clear();
		}

		// Basic rendering functions
		virtual void RenderRO(const RenderObject& ro, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
		virtual void RenderMesh(const std::shared_ptr<Luxia::Mesh> m_mesh, const std::shared_ptr<Luxia::IMaterial> m_material, const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
		virtual void RenderMeshPure(const Mesh& m_mesh) = 0;
		virtual void RenderFBO(const Mesh& m_quad, std::shared_ptr<IShader> fs_shader, std::shared_ptr<ITexture> cam_tex) = 0;
	
		// UI Renderer access
		std::shared_ptr<IUIRenderer> GetUIRenderer() { return m_UIRenderer; }
		std::shared_ptr<Luxia::IShader> default_shader = nullptr;
	protected:
		std::vector<RenderObject> renderObjects;
		std::shared_ptr<IUIRenderer> m_UIRenderer = nullptr;
	};
}