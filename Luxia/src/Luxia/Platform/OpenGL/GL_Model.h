#pragma once

#include "Luxia/Rendering/API/IModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Luxia/Components/Transform.h"

namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Model : public IModel {
	public:
		GL_Model() = default;
		~GL_Model() = default;

		virtual std::vector<std::shared_ptr<Mesh>> LoadFromPath(const std::filesystem::path& src_path) override;
		virtual bool Unload() override; // Implement?
	
	protected:
		void processNode(aiNode* node, const aiScene* ai_scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* ai_scene);
		std::vector<std::shared_ptr<ITexture>> loadTextures(aiMaterial* material, aiTextureType type);
	};
}