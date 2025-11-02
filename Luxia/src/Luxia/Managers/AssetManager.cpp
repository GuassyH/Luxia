#include "AssetManager.h"

namespace Luxia {

	bool AssetManager::LoadAssetPoolFromPath(const std::filesystem::path& m_path) { 

		return true; 
	}

	bool AssetManager::SaveAssetPool(const std::filesystem::path& m_path) {
		return true;
	}

	void AssetManager::Cleanup() {
		// Unload all assets
		for (auto& [path, asset] : loaded_assets) {
			if (asset && asset->loaded) {
				asset->Unload();
			}
		}

		// Clear the unordered_map (kill the shared_ptrs)
		loaded_assets.clear();
		LX_CORE_TRACE("Asset Manager cleaned up");
	}

}