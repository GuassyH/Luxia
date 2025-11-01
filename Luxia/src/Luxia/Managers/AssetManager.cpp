#include "AssetManager.h"

namespace Luxia {


	void AssetManager::Init() {
		// Lookup savefile
		// Get all saved assets
		// load them
	}

	void AssetManager::Cleanup() {
		// Save all assets first
		// for (auto& [path, asset] : loaded_assets) { /*save*/ }


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

	bool AssetManager::IsLoaded(std::string& path) {
		return loaded_assets.contains(path);
	}

}