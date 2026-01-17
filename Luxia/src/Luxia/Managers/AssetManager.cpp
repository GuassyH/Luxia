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

				asset_file->metaGUID = meta_file->guid;
				meta_file->assetGUID = asset_file->guid;

				// Assign both files pools
				assetfile_pool[asset_file->guid] = asset_file;
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

			meta_pool.find(asset_file->metaGUID)->second->Save();
			if (asset_file) {
				asset_file->Save(meta_pool.find(asset_file->metaGUID)->second->assetPath);
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
		{Luxia::AssetType::AudioType, ".luxaudio"},
	};

	// Renames both the asset file and the metafile, and updates the asset and metafile names
	bool AssetManager::RenameAsset(GUID assetGUID, std::string newName) {
		// O(1) lookup time

		auto it = asset_pool.find(assetGUID);
		if (it == asset_pool.end() || !it->second) {
			LX_CORE_ERROR("RenameAsset: Invalid asset GUID {}", (uint64_t)assetGUID);
			return false;
		}
		auto asset = it->second;
		if(!asset) { 
			LX_CORE_ERROR("RenameAsset: Asset is nullptr for GUID {}", (uint64_t)assetGUID);
			return false;
		}

		// Meshes are internal, so just rename the asset
		if(asset->type == AssetType::MeshType) {
			asset->name = newName;
			return true;
		}
		
		// Get assetfile and metafile
		std::shared_ptr<Assets::AssetFile> assetfile = nullptr;
		std::shared_ptr<Assets::MetaFile> metafile = nullptr;

		assetfile = assetfile_pool.find(asset->assetFileGUID)->second;
		metafile = meta_pool.find(assetfile->metaGUID)->second;

		auto af_it = assetfile_pool.find(asset->assetFileGUID);
		if (af_it == assetfile_pool.end() || !af_it->second) {
			LX_CORE_ERROR("RenameAsset: AssetFile not found for asset GUID {}", (uint64_t)assetGUID);
			return false;
		}
		assetfile = af_it->second;

		auto mf_it = meta_pool.find(assetfile->metaGUID);
		if (mf_it == meta_pool.end() || !mf_it->second) {
			LX_CORE_ERROR("RenameAsset: MetaFile not found for asset GUID {}", (uint64_t)assetGUID);
			return false;
		}
		metafile = mf_it->second;

		// Make assetfile path
		std::filesystem::path old_astfile_p = assetfile->assetPath;
		std::filesystem::path new_astfile_p = old_astfile_p;
		std::filesystem::path afext = old_astfile_p.extension();
		new_astfile_p.replace_filename(newName + afext.string());
		

		// Make Metafile path
		std::filesystem::path old_metafile_p = metafile->metaPath;
		std::filesystem::path new_metafile_p = old_metafile_p;
		std::filesystem::path mfext = old_metafile_p.extension();
		new_metafile_p.replace_filename(newName + mfext.string());

		// Make sure neither path exists already
		if(std::filesystem::exists(new_astfile_p)) {
			LX_ERROR("Cannot rename asset, asset file with name {} already exists", new_astfile_p.string());
			return false;
		}
		if (std::filesystem::exists(new_metafile_p)) {
			LX_ERROR("Cannot rename asset, meta file with name {} already exists", new_metafile_p.string());
			return false;
		}

		// Actually rename everything
		std::error_code assetfile_ec;
		std::filesystem::rename(old_astfile_p, new_astfile_p, assetfile_ec);
		if (assetfile_ec) {
			// If theres an error, reset the name to its original
			std::error_code rollback_ec;
			std::filesystem::rename(new_astfile_p, old_astfile_p, rollback_ec);

			LX_ERROR("Failed to rename asset file: {}", assetfile_ec.message());
			return false;
		}

		std::error_code metafile_ec;
		std::filesystem::rename(old_metafile_p, new_metafile_p, metafile_ec);
		if (metafile_ec) {
			// If theres an error, reset the name to its original
			std::error_code rollback_ec;
			std::filesystem::rename(new_metafile_p, old_metafile_p, rollback_ec);

			LX_ERROR("Failed to rename meta file: {}", metafile_ec.message());
			return false;
		}

		assetfile->assetPath = new_astfile_p;

		metafile->assetPath = new_astfile_p;
		metafile->metaPath = new_metafile_p;

		asset->name = newName;

		metafile->Save();
		assetfile->Save();

		return true;
	}


	bool AssetManager::DeleteAsset(GUID assetGUID) {
		if (!asset_pool.contains(assetGUID)) return false;

		// Check for no mesh
		auto asset = asset_pool.at(assetGUID);
		if (asset->type == Luxia::AssetType::MeshType) {
			LX_CORE_ERROR("AssetManager: Deleting meshes currently not allowed");
			return false;
		}

		if (!assetfile_pool.contains(asset->assetFileGUID)) { LX_CORE_ERROR("AssetManager: (Delete Failed) AssetFile not found for guid : {}", (uint64_t)asset->assetFileGUID); return false; }
		auto assetfile = assetfile_pool.at(asset->assetFileGUID);

		if (!meta_pool.contains(assetfile->metaGUID)) { LX_CORE_ERROR("AssetManager: (Delete Failed) MetaFile not found for guid : {}", (uint64_t)assetfile->metaGUID); return false; }
		auto metafile = meta_pool.at(assetfile->metaGUID);

		if (!metafile || !assetfile) { LX_CORE_ERROR("AssetManager: (Delete Failed) Meta or Asset- file nullptr"); return false; }

		asset->Unload();
		assetfile->Unload();
		metafile->Unload();

		std::remove(metafile->assetPath.string().c_str());
		std::remove(metafile->metaPath.string().c_str());

		asset_pool.erase(asset->guid);
		assetfile_pool.erase(assetfile->guid);
		meta_pool.erase(metafile->guid);

		return true;
	}
}