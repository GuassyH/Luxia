#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API MeshRenderer : public Component {
		// Cant be weak ptr since when it goes out of scope the weakptr goes null
		std::shared_ptr<Luxia::Mesh> mesh = nullptr;
		std::shared_ptr<Luxia::IMaterial> material = nullptr;

		MeshRenderer() = default;
		MeshRenderer(const std::shared_ptr<Luxia::Mesh>& m_mesh, const std::shared_ptr<IMaterial>& m_material) {
			mesh = m_mesh;
			material = m_material;
		}

		virtual void OnInspectorDraw() override {
		
		}
	};
}