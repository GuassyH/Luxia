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
		

		template <typename T>
		std::shared_ptr<T> Import(const std::filesystem::path& m_path) {

			return nullptr;
		}



		template <typename T, typename... Args> // Load an asset from a path
		std::shared_ptr<T> Create(const std::string& sourcePath, Luxia::AssetType type, Args&&... args) {
			std::filesystem::path full_path = asset_dir.string() + std::string("/") + sourcePath;

			if (loaded_assets.contains(full_path)) {
				LX_CORE_WARN("Asset at path: {}. Already exists!", full_path.string());
				return std::static_pointer_cast<T>(loaded_assets[full_path]);
			}

			std::shared_ptr<T> asset = nullptr;

			switch (type) {
			case Luxia::AssetType::Model: {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				model->LoadFromFile(full_path);
				asset = model;
				break;
			}
			default: {
				break;
			}
			}

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
			asset->~Asset();

			loaded_assets.erase(full_path);
		}

		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::Asset>> loaded_assets;
		std::unordered_map<std::filesystem::path, std::shared_ptr<Assets::AssetFile>> asset_pool;

		std::filesystem::path asset_dir;
	};

};