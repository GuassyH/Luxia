#include "lxpch.h"
#include "AssetManager.h"


namespace Luxia {

	bool AssetManager::LoadAssetPool(const std::filesystem::path& m_path) {
		asset_dir = m_path / "assets";
		asset_dir = asset_dir.lexically_normal();

		// Check if the dir exists already
		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_INFO("Asset Manager: creating asset dir - {}", asset_dir.string());

			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
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
				else {
					LX_CORE_TRACE("Loaded MetaFile - {}", path.string());
				}
				
				std::shared_ptr<Assets::AssetFile> asset_file = LoadAssetFile(meta_file->srcPath, meta_file->type);
				if (!asset_file) {
					LX_CORE_ERROR("Asset Manager: LoadAssetPool failed to load assetfile for - {}", meta_file->srcPath.string());
					continue;
				}
				else {
					LX_CORE_TRACE("Loaded AssetFile - {}", asset_file->assetPath.string());
				}

				// Assign both files pools
				asset_pool[meta_file->guid] = asset_file;
				meta_pool[meta_file->guid] = meta_file;
			}
		}


		return true; 
	}

	bool AssetManager::SaveAssetPool() {

		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_ERROR("Asset Manager: creating asset dir - {}", asset_dir.string());

			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
		}

		for (auto& [guid, asset_file] : asset_pool) {
			if (asset_file) {
				asset_file->Save(meta_pool.find(guid)->second->srcPath);
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
		{Luxia::AssetType::TextureType, ".luxtexture"},
		{Luxia::AssetType::MaterialType, ".luxmaterial"},
	};


}