#include "AssetManager.h"

namespace Luxia {


	bool AssetManager::IsLoaded(std::string& path) {
		return loaded_assets.contains(path);
	}

}