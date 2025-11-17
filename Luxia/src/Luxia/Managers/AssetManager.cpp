#include "lxpch.h"
#include "AssetManager.h"

namespace Luxia {

	bool AssetManager::LoadAssetPoolFromPath(const std::filesystem::path& m_path) { 
		asset_dir = m_path.string() + "/assets";

		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_INFO("Asset Manager: creating asset dir - {}", asset_dir.string());

			// Check if creating folder is successful
			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
		}

		// Load
		// foreach metafile, import.

		return true; 
	}

	bool AssetManager::SaveAssetPool(const std::filesystem::path& m_path) {
		asset_dir = m_path.string() + "/assets";

		if (!std::filesystem::exists(asset_dir)) {
			LX_CORE_ERROR("Asset Manager: creating asset dir - {}", asset_dir.string());

			// Check if creating folder is successful
			if (!std::filesystem::create_directory(asset_dir)) {
				LX_CORE_ERROR("Asset Manager: failed to create assets dir at - {}", asset_dir.string());
				return false;
			}
		}

		return true;
	}

	void AssetManager::Cleanup() {

		// Unload all assets
		for (auto& [path, asset] : loaded_assets) {
			if (asset) 
				asset->~Asset();
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
		{".gltf",  Luxia::AssetType::Model }
	};

	GUID AssetManager::Import(const std::string& m_path) {
		std::filesystem::path full_path = asset_dir.string() + std::string("/") + m_path;

		if (!full_path.has_extension()) { return 0; }

		std::shared_ptr<Luxia::Assets::AssetFile> asset_file = nullptr;

		Luxia::AssetType type = Luxia::AssetType::NoAsset;
		std::string af_ext = full_path.extension().string();

		for (auto& [ext, ext_type] : extensions) {
			if (af_ext.compare(ext)) {
				type = ext_type;
				break;
			}
		}

		if (type == Luxia::AssetType::NoAsset) { return 0; }

		switch (type)
		{
		case Luxia::NoAsset:
			break;
		case Luxia::Model:
			asset_file = std::make_shared<Luxia::Assets::ModelAsset>();
			break;
		case Luxia::Texture:
			asset_file = std::make_shared<Luxia::Assets::TextureAsset>();
			break;
		case Luxia::Material:
			break;
		case Luxia::Shader:
			break;
		case Luxia::Audio:
			break;
		default:
			break;
		}

		// if no metafile
		asset_file->Create(full_path);
		asset_file->srcPath = full_path;
		asset_file->extension = full_path.extension().string();
		asset_file->name = full_path.filename().string();



		asset_pool[asset_file->guid] = asset_file;
		LX_CORE_INFO("GUID: {}", (uint64_t)asset_file->guid);

		// return guid
		return asset_file->guid;
	}


}