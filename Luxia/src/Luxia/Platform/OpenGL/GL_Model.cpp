#include "lxpch.h"
#include "GL_Model.h"
#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Scene//Scene.h"

namespace Luxia::Platform::OpenGL {

	std::vector<std::shared_ptr<ITexture>> GL_Model::loadTextures(aiMaterial* material, aiTextureType type) {
		std::vector<std::shared_ptr<ITexture>> textures;

		for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);

			// prevent duplicates
			bool skip = false;

			for (unsigned int j = 0; j < textures.size(); j++) {
				if (std::strcmp(loaded_textures[j]->path.filename().string().c_str(), str.C_Str()) == 0) {
					textures.push_back(loaded_textures[j]);
					skip = true;
					break;
				}
			}

			if (!skip) {
				// not loaded
				std::shared_ptr<ITexture> tex = Platform::Assets::CreateTexture();
				
				std::ostringstream pstr; pstr << directory.string() << "/" << std::string(str.C_Str());
				std::filesystem::path new_p = pstr.str();

				tex->LoadFromFile(new_p, false); // type
				
				switch (type) {
				case aiTextureType_DIFFUSE:
					tex->type = TextureType::LX_TEXTURE_DIFFUSE;
					break;
				case aiTextureType_SPECULAR:
					tex->type = TextureType::LX_TEXTURE_SPECULAR;
					break;
				case aiTextureType_NORMALS:
					tex->type = TextureType::LX_TEXTURE_NORMALS;
					break;
				}

				textures.push_back(tex); 
				loaded_textures.push_back(tex);
			}
		}

		return textures;
	}


	std::vector<std::shared_ptr<Mesh>> GL_Model::LoadFromPath(const std::filesystem::path& src_path) {
		Assimp::Importer import;
		path = src_path;

		const aiScene* scene = import.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
		std::vector<std::shared_ptr<Mesh>>().swap(meshes);

		valid = false;

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LX_CORE_ERROR("Could not load model at {}\n{}", path.string().c_str(), import.GetErrorString());
			return meshes;
		}

		valid = true;

		directory = path.parent_path();
		name = scene->mName.C_Str();
		processNode(scene->mRootNode, scene);
		return meshes;
	}

	void GL_Model::processNode(aiNode* node, const aiScene* ai_scene) {
		// process all meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = ai_scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, ai_scene));
		}
			
		// process child nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], ai_scene);
		}
	}

	std::shared_ptr<Mesh> GL_Model::processMesh(aiMesh* mesh, const aiScene* ai_scene) {
		std::vector<Luxia::Rendering::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<std::shared_ptr<ITexture>> textures;

		// vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Luxia::Rendering::Vertex vertex = {};

			// Position
			vertex.pos = glm::vec3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

			// Normal
			vertex.normal = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);

			// Textures
			if (mesh->mTextureCoords[0]) {
				vertex.texCoords = glm::vec2(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				);
			}
			else {
				vertex.texCoords = glm::vec2(0.0f);
			}

			vertices.push_back(vertex);
		}


		// indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}


		std::shared_ptr<Luxia::IMaterial> mat = Platform::Assets::CreateMaterial();
		
		// Texture
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = ai_scene->mMaterials[mesh->mMaterialIndex];

			// diffuse
			std::vector<std::shared_ptr<ITexture>> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			mat->diffuse_texture = diffuseMaps.size() > 0 ? diffuseMaps[0] : nullptr;

			// specular
			std::vector<std::shared_ptr<ITexture>> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			mat->specular_texture = specularMaps.size() > 0 ? specularMaps[0] : nullptr;

			// normals
			std::vector<std::shared_ptr<ITexture>> normalMaps = loadTextures(material, aiTextureType_NORMALS);
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			mat->normal_texture = normalMaps.size() > 0 ? normalMaps[0] : nullptr;
		}

		std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(vertices, indices);
		newMesh->CalculateMesh();
		newMesh->name = mesh->mName.C_Str();
		newMesh->local_id = meshes.size();
		// Create mesh entity and said parent to root_entity
		return newMesh;
	}


	bool GL_Model::Unload() {
		for (std::shared_ptr<Mesh> mesh : meshes) {
			mesh->Unload();
		}
		return true;
	}

}