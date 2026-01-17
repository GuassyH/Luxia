#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include "Luxia/Core/WeakPtrProxy.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API MeshRenderer : public Component {
		WeakPtrProxy<Luxia::Mesh> mesh = nullptr;
		WeakPtrProxy<Luxia::IMaterial> material = nullptr;

		MeshRenderer() = default;
		MeshRenderer(const std::shared_ptr<Luxia::Mesh>& m_mesh, const std::shared_ptr<IMaterial>& m_material) {
			mesh = m_mesh;
			material = m_material;
		}

		virtual void OnInspectorDraw() override {
		
		}
	};
}