#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Asset/Asset.h"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Luxia {
	class LUXIA_API IMaterial : public Luxia::Assets::Asset {
	public:
		IMaterial() {
			type = Luxia::AssetType::MaterialType;
		}
		IMaterial(const std::shared_ptr<IShader> matShader) {
			shader = matShader;
			type = Luxia::AssetType::MaterialType;
		}
		virtual ~IMaterial() = default;

		virtual void Use(const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) = 0;
		virtual bool Unload() override { return true; }

		std::shared_ptr<IShader> shader = nullptr;

		std::shared_ptr<ITexture> diffuse_texture = nullptr;
		std::shared_ptr<ITexture> specular_texture = nullptr;
		std::shared_ptr<ITexture> normal_texture = nullptr;

		glm::vec4 color = glm::vec4(1.0f);
		float roughness = 1.0f;
		float metallic = 0.0f;
	}; 
}