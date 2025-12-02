#pragma once

#include "Luxia/Rendering/API/IModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Model : public IModel {
	public:
		GL_Model() = default;
		~GL_Model() = default;

		virtual void LoadFromFile(const std::shared_ptr<Luxia::Assets::ModelFile> model_asset, Scene* active_scene) override;
		virtual bool Unload() override; // Implement?
	
	protected:
		void processNode(aiNode* node, const aiScene* ai_scene, Scene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* ai_scene, Scene* scene);
		std::vector<std::shared_ptr<ITexture>> loadTextures(aiMaterial* material, aiTextureType type);
	};
}