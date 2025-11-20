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
				// asset->Cleanup?
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


	static std::unordered_map<std::string, Luxia::AssetType> extensions = {
		{".png",  Luxia::AssetType::Texture},
		{".jpg",  Luxia::AssetType::Texture},
		{".jpeg", Luxia::AssetType::Texture},
		{".bmp",  Luxia::AssetType::Texture},
		{".obj",  Luxia::AssetType::Model},
		{".fbx",  Luxia::AssetType::Model},
		{".gltf",  Luxia::AssetType::Model},
		{".wav",  Luxia::AssetType::Audio},
		{".mp3",  Luxia::AssetType::Audio},
		// {".scene", Luxia::AssetType::Scene}
	};

	std::shared_ptr<Luxia::Assets::AssetFile> AssetManager::Import(const std::filesystem::path& src_path, const std::string ast_name) {
		std::filesystem::path full_path = asset_dir.string() + "/" + src_path.string();

		if (!full_path.has_extension()) { return 0; }

		std::shared_ptr<Luxia::Assets::AssetFile> asset_file = std::make_shared<Luxia::Assets::AssetFile>();

		// Check if there is metafile
		bool has_metafile = false;
		std::filesystem::path metafile_path = full_path;
		metafile_path.replace_extension(".meta");

		if (std::filesystem::exists(metafile_path)) {
			has_metafile = true;
		}

		// If there isnt a metafile, create a new assetfile
		if (!has_metafile) {
			Luxia::AssetType type = Luxia::AssetType::NoAsset;
			std::string af_ext = full_path.extension().string();
			for (auto& [ext, ext_type] : extensions) {
				if (af_ext == ext) {
					type = ext_type;
					break;
				}
			}
			if (type == Luxia::AssetType::NoAsset) { return 0; }

			// If assetfile creation failed, return nullptr
			if (!asset_file->Create(full_path, metafile_path, ast_name, type)) { return nullptr; }
		}
		// If there is one, reimport the assetfile (save with new path / name) but keep GUID
		else {
			asset_file->srcPath = full_path;
			asset_file->name = ast_name;

			// If the assetfile couldnt load, return nullptr
			if (!asset_file->Load(metafile_path)) { return nullptr; }
		}

		// Set asset_pool entry
		asset_pool[asset_file->guid] = asset_file;

		return asset_file;
	}

	std::shared_ptr<Luxia::Assets::AssetFile> AssetManager::SerializeAssetFile(const std::filesystem::path& metafile_path) {
		// Make sure meta_path is valid
		if (!metafile_path.has_extension() || !std::filesystem::exists(metafile_path)) { return 0; }

		std::shared_ptr<Luxia::Assets::AssetFile> asset_file = std::make_shared<Luxia::Assets::AssetFile>();

		// If failed a load, return nullptr
		if (!asset_file->Load(metafile_path)) { return nullptr; }
		
		return asset_file;
	}


	std::shared_ptr<Luxia::Assets::AssetFile> AssetManager::GetAssetFileFromPath(const std::filesystem::path& m_path) {
		std::filesystem::path full_path = asset_dir.string() + std::string("/") + m_path.string();

		for (auto& [t_guid, ast_file] : asset_pool) {
			if (ast_file->srcPath == full_path) {
				LX_CORE_TRACE("Found assetfile from path!: {}", m_path.string());
				return ast_file;
			}
		}

		return nullptr;
	}

}