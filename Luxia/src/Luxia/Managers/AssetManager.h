#pragma once

#include "Luxia/Core.h"
#include "Luxia/Log.h"

#include "Luxia/Assets/Asset.h"
#include "Luxia/Assets/TextureAsset.h"
#include "Luxia/Assets/ModelAsset.h"

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <filesystem>



namespace Luxia {

	class LUXIA_API AssetManager {
	public:
		void Init(); // When app opens
		void Cleanup(); // Before app closes, save everything, etc

		int NumLoaded() { return loaded_assets.size(); }
		bool IsLoaded(const std::filesystem::path& m_path) { return loaded_assets.contains(m_path); }
		bool LoadAssetPoolFromPath(const std::filesystem::path& m_path) { return true; }
		bool SaveAssetPool(const std::filesystem::path& m_path) { return true; }

		template <typename T> // Load an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, std::shared_ptr<T>> 
			Load(const std::filesystem::path& m_path) {
			if (loaded_assets.contains(m_path)) {
				LX_CORE_WARN("Asset at path: {}. Already exists!", m_path.string());
				return std::static_pointer_cast<T>(loaded_assets[m_path]);
			}

			auto asset = std::make_shared<T>();

			if (asset->type == Assets::AssetType::NoAsset) {
				LX_CORE_ERROR("Asset at path: {}. Does not have asset type!", m_path.string());
				return asset;
			}

			asset->Load(m_path);
			loaded_assets[m_path] = asset;

			return asset;
		}

		template <typename T> // Unload an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, void> 
			Unload(const std::filesystem::path& m_path) {
			if (!loaded_assets.contains(m_path)) {
				LX_CORE_TRACE("Asset ({}) already unloaded", m_path.string());
				return;
			}

			auto asset = loaded_assets.find(m_path)->second;
			asset->Unload();

			loaded_assets.erase(m_path);
		}

		~AssetManager() = default;
		AssetManager() = default;

	private:
		// std::unordered_set<std::string> loaded_paths; // Stores the path
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> loaded_assets;
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> asset_pool;
	};

};