#include "lxpch.h"
#include "AssetManager.h"


namespace Luxia {

	bool AssetManager::LoadAssetPoolFromPath(const std::filesystem::path& m_path) { 
		// Doing the m_path / "assets" made ...\assets for some reason
		asset_dir = m_path.string() + "/assets";

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
				// Replaces '\' with '/', '\\' means '\'
				std::string logPath = path.string();
				std::replace(logPath.begin(), logPath.end(), '\\', '/');

				// LX_CORE_TRACE("Loading meta file: {}", logPath);

				// Serializes (loads) Asset File at the path
				std::shared_ptr<Luxia::Assets::AssetFile> new_astf = SerializeAssetFile(logPath);

				if (new_astf)
					asset_pool[new_astf->guid] = new_astf;
				else
					LX_CORE_WARN("Failed to load asset from meta file: {}", logPath);
				
			}
		}


		return true; 
	}

	bool AssetManager::SaveAssetPool(const std::filesystem::path& m_path) {
		// Doing the m_path / "assets" made ...\assets for some reason
		asset_dir = m_path.string() + "/assets";

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
			if(asset_file)
				asset_file->Save();
		}

		return true;
	}

	void AssetManager::Cleanup() {
		// Unload all assets
		for (auto& [guid, asset] : loaded_assets) {
			if (asset) {
				asset->Unload();
				asset->~Asset();
			}
		}

		for (auto& [guid, assetfile] : asset_pool) {
			if (assetfile) {
				assetfile->Unload();
				assetfile->~AssetFile();
			}
		}

		// Clear the unordered_map (kill the shared_ptrs)
		loaded_assets.clear();
		asset_pool.clear();
		LX_CORE_TRACE("Asset Manager cleaned up");
	}

	
	std::unordered_map<std::string, Luxia::AssetType> AssetManager::extensions = {
		{".png",  Luxia::AssetType::Texture},
		{".jpg",  Luxia::AssetType::Texture},
		{".jpeg", Luxia::AssetType::Texture},
		{".bmp",  Luxia::AssetType::Texture},
		{".obj",  Luxia::AssetType::Model},
		{".fbx",  Luxia::AssetType::Model},
		{".gltf",  Luxia::AssetType::Model},
		{".wav",  Luxia::AssetType::Audio},
		{".mp3",  Luxia::AssetType::Audio},
		{".shader",  Luxia::AssetType::Shader},
	};

	std::shared_ptr<Luxia::Assets::AssetFile> AssetManager::SerializeAssetFile(const std::filesystem::path& metafile_path) {
		// Make sure meta_path is valid
		if (!metafile_path.has_extension() || !std::filesystem::exists(metafile_path)) { return nullptr; }

		std::shared_ptr<Luxia::Assets::AssetFile> asset_file = std::make_shared<Luxia::Assets::AssetFile>();

		// Loads the minimal, basic data
		Luxia::AssetType type = Luxia::PeakAssetType(metafile_path);

		switch (type)
		{
		case Luxia::AssetType::Shader: 
			asset_file = std::make_shared<Luxia::Assets::ShaderFile>();
			break;
		case Luxia::AssetType::Texture:
			// asset_file = std::make_shared<Luxia::Assets::TextureFile>();
			asset_file = std::make_shared<Luxia::Assets::AssetFile>();
			break;

		case Luxia::AssetType::Model:
			// asset_file = std::make_shared<Luxia::Assets::ModelFile>();
			asset_file = std::make_shared<Luxia::Assets::AssetFile>();
			break;
		default:
			LX_CORE_ERROR("Unknown asset type in metafile '{}'", metafile_path.string());
			return nullptr;
		}

		// Loads the data specific to shader, texture, etc
		asset_file->Load(metafile_path);
		
		return asset_file;
	}

}