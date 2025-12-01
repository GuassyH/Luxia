#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Asset/MaterialFile.h"
#include <iostream>

#include "Component.h"

namespace Luxia::Components {
	struct LUXIA_API Material : public Component {
		std::shared_ptr<Luxia::ITexture> diffuse_texture = nullptr;
		std::shared_ptr<Luxia::ITexture> specular_texture = nullptr;
		std::shared_ptr<Luxia::ITexture> normal_texture = nullptr;
		
		glm::vec4 color = glm::vec4(1.0f);
		float roughness = 1.0f;
		float metallic = 0.0f;

		std::shared_ptr<IShader> shader;
		Material(std::shared_ptr<IShader> m_shader) :
			shader(m_shader) {
		}
		Material(std::shared_ptr<Luxia::Assets::MaterialFile> matFile, std::shared_ptr<IShader> m_shader) : 
			color(matFile->color), 
			roughness(matFile->roughness),
			metallic(matFile->metallic),
			shader(m_shader) { 
			// Set diffuse and allat
		}

		void Use() {
			shader->Use();
			shader->SetVec4("mat_color", color);
			shader->SetFloat("mat_roughness", roughness);
			shader->SetFloat("mat_metallic", metallic);

			// Bind textures if exist
			if (diffuse_texture) {
				// Active Texture Unit 0
				diffuse_texture->Use();
				shader->SetInt("diffuse0", 0);
			}
			if (specular_texture) {
				// Active Texture Unit 1
				specular_texture->Use();
				shader->SetInt("specular0", 0);
			}
			if (normal_texture) {
				// Active Texture Unit 2
				normal_texture->Use();
				shader->SetInt("normals0", 0);
			}
		}
	};
}