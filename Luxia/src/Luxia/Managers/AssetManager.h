#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/TextureAsset.h"
#include "Luxia/Asset/ModelAsset.h"
#include "Luxia/Asset/Asset.h"

#include "Luxia/Platform/PlatformDefinitions.h"

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
		const std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>>& GetAssets() { return loaded_assets; }

		void Cleanup(); // Before app closes, save everything, etc
		GUID Import(const std::string& m_path);
		

		template <typename T, typename... Args> // Load an asset from a path
		std::shared_ptr<T> CreateAsset(const GUID& assetfile_guid, Args&&... args) {
			if (!asset_pool.contains(assetfile_guid)) { LX_CORE_ERROR("Asset pool doesnt have {}", (uint64_t)assetfile_guid); return nullptr; }
			std::shared_ptr<Assets::AssetFile> ast_file = asset_pool.find(assetfile_guid)->second;

			if (loaded_assets.contains(ast_file->srcPath)) {
				LX_CORE_WARN("Asset at path: {}. Already exists!", 0);
				return std::static_pointer_cast<T>(loaded_assets[ast_file->srcPath.string()]);
			}

			std::shared_ptr<T> asset = nullptr;

			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				model->LoadFromFile(ast_file->srcPath);
				asset = model;
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				std::shared_ptr<Luxia::ITexture> texture = Luxia::Platform::Assets::CreateTexture();
				texture->LoadFromFile(ast_file->srcPath);
				asset = texture;
			}
			

			loaded_assets[ast_file->srcPath] = asset;
			return asset;
		}



		template <typename T> // Unload an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, void>
			UnloadAsset(const std::string& m_path) {
			std::filesystem::path full_path = asset_dir.string() + std::string("/") + m_path;

			if (!loaded_assets.contains(full_path)) {
				LX_CORE_TRACE("Asset ({}) already unloaded", full_path.string());
				return;
			}

			auto& asset = loaded_assets.find(full_path)->second;
			asset->~Asset();

			loaded_assets.erase(full_path);
		}

		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> loaded_assets;
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>> asset_pool;

		std::filesystem::path asset_dir;
	};

};