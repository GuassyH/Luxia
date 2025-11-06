#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/IShader.h"
#include <iostream>

namespace Luxia::Components {
	struct LUXIA_API MeshRenderer {
		std::shared_ptr<IModel> model;
		std::shared_ptr<IShader> shader;

		MeshRenderer(std::shared_ptr<IModel> m_model, std::shared_ptr<IShader> m_shader) :
			model(m_model), shader(m_shader) {}
	};
}