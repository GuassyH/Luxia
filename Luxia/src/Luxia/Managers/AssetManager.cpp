#include "lxpch.h"
#include "AssetManager.h"


namespace Luxia {

	bool AssetManager::LoadAssetPool(const std::filesystem::path& m_path) {
		asset_dir = m_path / "assets";
		asset_dir = asset_dir.lexically_normal();

		// Check if the dir exists already
		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_INFO("Asset Manager: creating asset dir - {}", asset_dir.string());
		}

		// Load: iterate through all .meta files
		for (auto& entry : std::filesystem::recursive_directory_iterator(asset_dir)) {
			if (!entry.is_regular_file()) continue;

			auto path = entry.path().lexically_normal();
			if (path.extension() == ".meta") {
				std::shared_ptr<Assets::MetaFile> meta_file = LoadMetaFile(path);
				if (!meta_file) {
					LX_CORE_ERROR("Asset Manager: LoadAssetPool failed to load metafile for - {}", path.string());
					continue;
				}
				
				std::shared_ptr<Assets::AssetFile> asset_file = LoadAssetFile(meta_file->assetPath, meta_file->type);
				if (!asset_file) {
					LX_CORE_ERROR("Asset Manager: LoadAssetPool failed to load assetfile for - {}", meta_file->assetPath.string());
					continue;
				}

				asset_file->guid = meta_file->guid;

				// Assign both files pools
				assetfile_pool[meta_file->guid] = asset_file;
				meta_pool[meta_file->guid] = meta_file;
			}
		}

		// Iterate through assets and do what you need

		// Since shaders should be loaded now, we can now load the material files
		for (auto [guid, assetfile] : assetfile_pool) {
			if (assetfile->type == Luxia::AssetType::MaterialType) {
				std::shared_ptr<Luxia::Assets::MaterialFile> matfile = std::dynamic_pointer_cast<Luxia::Assets::MaterialFile>(assetfile);
				std::shared_ptr<Luxia::IMaterial> mat = std::dynamic_pointer_cast<Luxia::IMaterial>(assetfile->assets[0]);
				if (matfile && mat) {
					if (HasAsset<Luxia::IShader>(matfile->shaderGUID)) 
						mat->shader = GetAsset<Luxia::IShader>(matfile->shaderGUID);
					else 
						LX_CORE_ERROR("AssetManager: Failed loading Material Shader, Shader GUID {} doesnt exist", (uint64_t)matfile->shaderGUID);

					if (HasAsset<Luxia::ITexture>(matfile->diffuseGUID))		// Get Texture
						mat->diffuse_texture = GetAsset<Luxia::ITexture>(matfile->diffuseGUID);
					if (HasAsset<Luxia::ITexture>(matfile->specularGUID))		// Get Texture
						mat->specular_texture = GetAsset<Luxia::ITexture>(matfile->specularGUID);
					if (HasAsset<Luxia::ITexture>(matfile->normalsGUID))		// Get Texture
						mat->normal_texture = GetAsset<Luxia::ITexture>(matfile->normalsGUID);
				}
				else {
					LX_CORE_ERROR("AssetManager: Failed loading Material {}", (uint64_t)matfile->guid);
				}
			}
		}

		return true; 
	}

	bool AssetManager::SaveAssetPool(const std::filesystem::path& m_path) {
		asset_dir = m_path / "assets";
		asset_dir = asset_dir.lexically_normal();

		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_ERROR("Asset Manager: creating asset dir - {}", asset_dir.string());

			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
		}

		// Save metafile, then save asset_file
		for (auto& [guid, asset_file] : assetfile_pool) {

			meta_pool.find(guid)->second->Save();
			if (asset_file) {
				asset_file->Save(meta_pool.find(guid)->second->assetPath);
			}
		}

		return true;
	}

	void AssetManager::Cleanup() {
		for (auto& [guid, assetfile] : asset_pool) {
			if (assetfile) {
				assetfile->Unload();
			}
		}

		asset_pool.clear();
		LX_CORE_TRACE("Asset Manager cleaned up");
	}

	
	std::unordered_map<std::string, Luxia::AssetType> AssetManager::extensions = {
		{".png",	Luxia::AssetType::TextureType},
		{".jpg",	Luxia::AssetType::TextureType},
		{".jpeg",	Luxia::AssetType::TextureType},
		{".bmp",	Luxia::AssetType::TextureType},
		{".obj",	Luxia::AssetType::ModelType},
		{".fbx",	Luxia::AssetType::ModelType},
		{".gltf",	Luxia::AssetType::ModelType},
		{".wav",	Luxia::AssetType::AudioType},
		{".mp3",	Luxia::AssetType::AudioType},
	};
	std::unordered_map<Luxia::AssetType, std::string> AssetManager::asset_extensions = {
		{Luxia::AssetType::ShaderType, ".luxshader"},
		{Luxia::AssetType::SceneType, ".luxscene"},
		{Luxia::AssetType::ModelType, ".luxmodel"},
		{Luxia::AssetType::TextureType, ".luxtex"},
		{Luxia::AssetType::MaterialType, ".luxmat"},
	};


}