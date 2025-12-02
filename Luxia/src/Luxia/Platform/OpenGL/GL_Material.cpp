#include "lxpch.h"
#include "GL_Material.h"
#include "glad/glad.h"
#include "Luxia/Platform/AssetCreation.h"

namespace Luxia::Platform::OpenGL {

	GL_Material::GL_Material() {
		shader = Platform::Assets::CreateShader("E:/BuiltLuxia/Sandbox/assets/shaders/default.frag", "E:/BuiltLuxia/Sandbox/assets/shaders/default.vert");
	}

	void GL_Material::LoadFromFile(const std::shared_ptr<Luxia::Assets::MaterialFile> matFile) {
		
	}

	void GL_Material::Use() {
		if (shader) {
			shader->Use();
			// Set material properties
			shader->SetVec4("mat_color", color);
			shader->SetFloat("mat_roughness", roughness);
			shader->SetFloat("mat_metallic", metallic);
			// Bind textures if they exist
			if (diffuse_texture && diffuse_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE0);
				diffuse_texture->Use();
				shader->SetInt("diffuse0", 0);
			}
			if (specular_texture && specular_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE1);
				specular_texture->Use();
				shader->SetInt("specular0", 1);
			}
			if (normal_texture && normal_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE2);
				normal_texture->Use();
				shader->SetInt("normals0", 2);
			}
		}
	}

	bool GL_Material::Unload() {

		return true;
	}


}