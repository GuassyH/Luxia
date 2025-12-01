#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Asset/MaterialFile.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API Material : public Component {
		glm::vec4 color = glm::vec4(1.0f);

		std::shared_ptr<IShader> shader;
		Material(std::shared_ptr<IShader> m_shader) :
			shader(m_shader) {
		}
		Material(std::shared_ptr<Luxia::Assets::MaterialFile> matFile, std::shared_ptr<IShader> m_shader) : 
			color(matFile->color), 
			shader(m_shader) { 
		}
	};
}