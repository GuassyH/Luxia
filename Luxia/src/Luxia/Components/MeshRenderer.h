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
		std::shared_ptr<IModel> model = nullptr;
		std::shared_ptr<IMaterial> material = nullptr;

		MeshRenderer() = default;
		MeshRenderer(const std::shared_ptr<Luxia::IModel>& m_model, const std::shared_ptr<IMaterial>& m_material) {
			model = m_model;
			material = m_material;
		}

	};
}