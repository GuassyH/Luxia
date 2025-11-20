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

		virtual void LoadFromFile(const std::filesystem::path& m_path) override;
		virtual bool Unload() override; // Implement?
	
	protected:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<ITexture>> loadTextures(aiMaterial* material, aiTextureType type);
	};
}