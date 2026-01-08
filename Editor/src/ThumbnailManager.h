#pragma once

#include "Luxia.h"

// This is not portable
#include "glad/glad.h"

namespace Editor {

	class ThumbnailManager {
	public:
		ThumbnailManager() = default;

		glm::ivec2 tn_size = glm::ivec2(256, 256);
		glm::vec4 bg_col = glm::vec4(0.1f, 0.1f, 0.2f, 1.0f);

		void Init(uint32_t w, uint32_t h) {
			tn_size = glm::vec2(w, h);
		}

		std::shared_ptr<Luxia::ITexture> TakeThumbnail(std::shared_ptr<Luxia::Assets::Asset> asset, Luxia::Rendering::IRenderer* renderer) {
			if (!asset || !renderer) return nullptr;

			std::shared_ptr<Luxia::ITexture> tex = Luxia::Platform::Assets::CreateTexture();


			switch (asset->type)
			{
			case Luxia::AssetType::MaterialType: {
				tex->CreateFBOTex(tn_size.x, tn_size.y);
				auto mat = std::dynamic_pointer_cast<Luxia::IMaterial>(asset);
				MatThumbnail(mat.get(), tex, renderer);
				break;
			}
			case Luxia::AssetType::MeshType: {
				tex->CreateFBOTex(tn_size.x, tn_size.y);
				auto mesh = std::dynamic_pointer_cast<Luxia::Mesh>(asset);
				MeshThumbnail(mesh.get(), tex, renderer);
				break;
			}
			default:
				break;
			}

			return tex->IsValid() ? tex : nullptr;
		}

		void RefreshThumbnail(std::shared_ptr<Luxia::Assets::Asset> asset, std::shared_ptr<Luxia::ITexture> texture, Luxia::Rendering::IRenderer* renderer) {
			switch (asset->type)
			{
			case Luxia::AssetType::MaterialType: {
				auto mat = std::dynamic_pointer_cast<Luxia::IMaterial>(asset);
				MatThumbnail(mat.get(), texture, renderer);
				break;
			}
			case Luxia::AssetType::MeshType: {
				auto mesh = std::dynamic_pointer_cast<Luxia::Mesh>(asset);
				MeshThumbnail(mesh.get(), texture, renderer);
				break;
			}
			default:
				break;
			}
		}

		void MatThumbnail(Luxia::IMaterial* mat, std::shared_ptr<Luxia::ITexture> texture, Luxia::Rendering::IRenderer* renderer) {

			glBindFramebuffer(GL_FRAMEBUFFER, texture->GetFBO());
			glViewport(0, 0, tn_size.x, tn_size.y);

			glEnable(GL_DEPTH_TEST);
			glClearColor(bg_col.r, bg_col.g, bg_col.b, bg_col.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::lookAt(glm::vec3(0.57735f, 0.57735f, 0.57735f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0,1,0));
			glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1.0f, 0.1f, 10.0f);

			if (Luxia::ResourceManager::DefaultSphere.get() && mat) 
				renderer->RenderMesh(Luxia::ResourceManager::DefaultSphere.get(), mat, model, view, proj);
			else
				LX_ERROR("Thumbnail for Mat (FBO: {}) failed", texture->GetFBO());


			LX_INFO("Created Thumbnail for Mat");
		}

		void MeshThumbnail(Luxia::Mesh* mesh, std::shared_ptr<Luxia::ITexture> texture, Luxia::Rendering::IRenderer* renderer) {
			glBindFramebuffer(GL_FRAMEBUFFER, texture->GetFBO());
			glViewport(0, 0, tn_size.x, tn_size.y);

			glEnable(GL_DEPTH_TEST);
			glClearColor(bg_col.r, bg_col.g, bg_col.b, bg_col.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// std::vector<Luxia::Rendering::Vertex> verts = mesh->vertices;
			float max_dist = 0;

			// Find the biggest distance to the origo
			for (auto& vert : mesh->vertices) {
				float dst = glm::distance(vert.pos, glm::vec3(0.0f));
				if (dst > max_dist) {
					max_dist = dst;
				}
			}

			max_dist = glm::abs(max_dist);

			glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / max_dist, 1.0f / max_dist, 1.0f / max_dist));
			glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
			glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1.0f, 0.1f, 10.0f);

			if (Luxia::ResourceManager::DefaultLitMaterial.get() && mesh)
				renderer->RenderMesh(mesh, Luxia::ResourceManager::DefaultLitMaterial.get(), model, view, proj);
			else
				LX_ERROR("Thumbnail for Mesh (FBO: {}) failed", texture->GetFBO());


			LX_INFO("Created Thumbnail for Mesh");
		}

	};

}