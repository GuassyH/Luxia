#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Assets/Asset.h"
#include "Luxia/Assets/TextureAsset.h"
#include "Luxia/Assets/ModelAsset.h"

#include <unordered_map>
#include <iostream>
#include <vector>

namespace Luxia {

	class LUXIA_API AssetManager {
	public:
		int NumLoaded() { return loaded_assets.size(); }
		bool IsLoaded(const std::filesystem::path& m_path) { return loaded_assets.contains(m_path); }
		bool LoadAssetPoolFromPath(const std::filesystem::path& m_path);
		bool SaveAssetPool(const std::filesystem::path& m_path);

		// TEMP, THIS SHOULD BE HANDLED BY SCENE
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> GetAssets() { return loaded_assets; }

		void Cleanup(); // Before app closes, save everything, etc
		

		template <typename T> // Load an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, std::shared_ptr<T>> 
			Create(const std::string& sourcePath) {
			std::filesystem::path full_path = asset_dir.string() + std::string("/") + sourcePath;

			if (loaded_assets.contains(full_path)) {
				LX_CORE_WARN("Asset at path: {}. Already exists!", full_path.string());
				return std::static_pointer_cast<T>(loaded_assets[full_path]);
			}

			auto asset = std::make_shared<T>();

			if (asset->type == Assets::AssetType::NoAsset) {
				LX_CORE_ERROR("Asset at path: {}. Does not have asset type!", full_path.string());
				return asset;
			}

			asset->Create(full_path);
			loaded_assets[full_path] = asset;

			return asset;
		}


		template <typename T> // Load an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, std::shared_ptr<T>>
			Load(const std::string& metaPath) {
			std::filesystem::path full_path = asset_dir.string() + std::string("/") + metaPath;

			if (loaded_assets.contains(full_path)) {
				LX_CORE_WARN("Asset at path: {}. Already exists!", full_path.string());
				return std::static_pointer_cast<T>(loaded_assets[full_path]);
			}

			auto asset = std::make_shared<T>();

			if (asset->type == Assets::AssetType::NoAsset) {
				LX_CORE_ERROR("Asset at path: {}. Does not have asset type!", full_path.string());
				return asset;
			}

			asset->Load(full_path);
			loaded_assets[full_path] = asset;

			return asset;
		}


		template <typename T> // Unload an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, void> 
			Unload(const std::string& m_path) {
			std::filesystem::path full_path = asset_dir.string() + std::string("/") + m_path;

			if (!loaded_assets.contains(full_path)) {
				LX_CORE_TRACE("Asset ({}) already unloaded", full_path.string());
				return;
			}

			auto& asset = loaded_assets.find(full_path)->second;
			asset->Unload();

			loaded_assets.erase(full_path);
		}

		~AssetManager() = default;
		AssetManager() = default;
	private:
		// std::unordered_set<std::string> loaded_paths; // Stores the path
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> loaded_assets;
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> asset_pool;

		std::filesystem::path asset_dir;
	};

};