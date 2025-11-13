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

}