#include "lxpch.h"
#include "GL_Material.h"
#include "glad/glad.h"
#include "Luxia/Platform/AssetCreation.h"

namespace Luxia::Platform::OpenGL {

	GL_Material::GL_Material() = default;

	void GL_Material::Use(const glm::mat4& modMat, const glm::mat4& viewMat, const glm::mat4& projMat) {
		if (shader) {
			shader->Use();
			// Set material properties
			shader->SetVec4("mat_color", color);
			shader->SetFloat("mat_roughness", roughness);
			shader->SetFloat("mat_metallic", metallic);


			shader->SetMat4("modelMat", modMat);
			shader->SetMat4("viewMat", viewMat);
			shader->SetMat4("projMat", projMat);

			int sampler_id = 1; // start from 1, 0 is reserved for other uses
			// Bind textures if they exist
			if (diffuse_texture && diffuse_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE0 + sampler_id);
				diffuse_texture->Use();
				shader->SetInt("diffuse0", sampler_id);
				sampler_id++;
			}
			if (specular_texture && specular_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE0 + sampler_id);
				specular_texture->Use();
				shader->SetInt("specular0", sampler_id);
				sampler_id++;
			}
			if (normal_texture && normal_texture->IsValid()) {
				glActiveTexture(GL_TEXTURE0 + sampler_id);
				normal_texture->Use();
				shader->SetInt("normals0", sampler_id);
				sampler_id++;
			}

			shader->SetBool("hasDiffuse", (diffuse_texture && diffuse_texture->IsValid()) ? true : false);
			shader->SetBool("hasSpecular", (specular_texture && specular_texture->IsValid()) ? true : false);
			shader->SetBool("hasNormals", (normal_texture && normal_texture->IsValid()) ? true : false);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	bool GL_Material::Unload() {

		return true;
	}


}