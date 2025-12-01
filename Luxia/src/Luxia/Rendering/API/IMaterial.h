#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Asset/MaterialFile.h"
#include "Luxia/Asset/Asset.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Luxia {
	class LUXIA_API IMaterial : public Luxia::Assets::Asset {
	public:
		IMaterial() = default;
		IMaterial(const std::shared_ptr<Assets::MaterialFile> matFile) {}
		IMaterial(const std::shared_ptr<IShader> matShader) {
			shader = matShader;
		}
		virtual ~IMaterial() = default;

		virtual void LoadFromFile(const std::shared_ptr<Assets::MaterialFile> matFile) = 0;
		virtual void Use() = 0;
		virtual bool Unload() override = 0;

		std::shared_ptr<IShader> shader = nullptr;

		std::shared_ptr<ITexture> diffuse_texture = nullptr;
		std::shared_ptr<ITexture> specular_texture = nullptr;
		std::shared_ptr<ITexture> normal_texture = nullptr;

		glm::vec4 color = glm::vec4(1.0f);
		float roughness = 1.0f;
		float metallic = 0.0f;
	}; 
}