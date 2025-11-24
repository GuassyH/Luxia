#include "lxpch.h"
#include "AssetManager.h"


namespace Luxia {

	bool AssetManager::LoadAssetPoolFromPath(const std::filesystem::path& m_path) { 
		// Doing the m_path / "assets" made ...\assets for some reason
		asset_dir = m_path / "assets";
		asset_dir = asset_dir.lexically_normal();

		LX_CORE_INFO(asset_dir.string());


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
				std::shared_ptr<Luxia::Assets::AssetFile> new_astf = SerializeAssetFile(path);

				if (new_astf)
					asset_pool[new_astf->guid] = new_astf;
				else
					LX_CORE_WARN("Failed to load asset from meta file: {}", path.string());
				
			}
		}


		return true; 
	}

	bool AssetManager::SaveAssetPool(const std::filesystem::path& m_path) {

		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_ERROR("Asset Manager: creating asset dir - {}", asset_dir.string());

			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
		}

		// For each asset, save
		for (auto& [guid, asset_file] : asset_pool) {
			// assert !nullptr
			if (asset_file) {
				asset_file->Save();
			}
		}

		return true;
	}

	void AssetManager::Cleanup() {
		// Unload all asset files
		for (auto& [guid, assetfile] : asset_pool) {
			if (assetfile) {
				assetfile->Unload();
			}
		}

		asset_pool.clear();
		LX_CORE_TRACE("Asset Manager cleaned up");
	}

	
	std::unordered_map<std::string, Luxia::AssetType> AssetManager::extensions = {
		{".png",	Luxia::AssetType::Texture},
		{".jpg",	Luxia::AssetType::Texture},
		{".jpeg",	Luxia::AssetType::Texture},
		{".bmp",	Luxia::AssetType::Texture},
		{".obj",	Luxia::AssetType::Model},
		{".fbx",	Luxia::AssetType::Model},
		{".gltf",	Luxia::AssetType::Model},
		{".wav",	Luxia::AssetType::Audio},
		{".mp3",	Luxia::AssetType::Audio},
		{".shader",	Luxia::AssetType::Shader},
	};

	std::shared_ptr<Luxia::Assets::AssetFile> AssetManager::SerializeAssetFile(std::filesystem::path& metafile_path) {
		if (!metafile_path.has_extension() || !std::filesystem::exists(metafile_path)) { return nullptr; }
		metafile_path = metafile_path.lexically_normal();

		std::shared_ptr<Luxia::Assets::AssetFile> asset_file = nullptr;

		Luxia::AssetType type = Luxia::PeakAssetType(metafile_path);

		asset_file = MakeSharedFileFromType(type);

		if (!asset_file) {
			LX_CORE_ERROR("Serializing nullptr");
			return nullptr;
		}

		asset_file->Load(metafile_path);

		// Loads the data specific to shader, texture, etc
		return asset_file;
	}

}