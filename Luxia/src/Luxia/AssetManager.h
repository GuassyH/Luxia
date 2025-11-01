#pragma once

#include "Luxia/Core.h"
#include "Luxia/Log.h"

#include "Luxia/Asset.h"
#include "Luxia/TextureAsset.h"

#include <unordered_set>
#include <unordered_map>
#include <iostream>



namespace Luxia {

	class LUXIA_API AssetManager {
	public:
		static AssetManager& Get() { static AssetManager instance; return instance; }

		// std::unordered_set<std::string> loaded_paths; // Stores the path
		std::unordered_map<std::string, std::shared_ptr<Asset>> loaded_assets;


		void Init() {}	// When app opens
		void Finish() {} // Before app closes, save everything, etc

		bool IsLoaded(std::string& m_path);

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Asset, T>, std::shared_ptr<T>> Load(std::string& m_path) {
			if (loaded_assets.contains(m_path))
				return std::static_pointer_cast<T>(loaded_assets[m_path]);

			auto asset = std::make_shared<T>();

			if (asset->type == AssetType::NoAsset) 
				LX_CORE_ERROR("Asset at path: {}. Does not have asset type!", m_path.c_str());

			asset->Load(m_path);
			loaded_assets[m_path] = asset;

			return asset;
		}

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Asset, T>, void> Unload(std::string& m_path) {
			if (!loaded_assets.contains(m_path)) {
				LX_CORE_TRACE("Asset ({}) already unloaded", m_path.c_str());
				return;
			}

			auto asset = loaded_assets.find(m_path)->second;
			asset->Unload();

			loaded_assets.erase(m_path);
		}

	private:
		AssetManager() = default;
	};

};