#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/Asset.h"

#include "Luxia/Platform/PlatformDefinitions.h"

#include <unordered_map>
#include <iostream>
#include <vector>

namespace Luxia {


	class LUXIA_API AssetManager {
	private:
		static std::unordered_map<std::string, Luxia::AssetType> extensions;
	public:
		int NumLoaded() { return loaded_assets.size(); }
		int NumAssetPool() { return asset_pool.size(); }

		bool LoadAssetPoolFromPath(const std::filesystem::path& m_path);
		bool SaveAssetPool(const std::filesystem::path& m_path);
		void Cleanup(); // Before app closes, save everything, etc

		const std::unordered_map<GUID, std::shared_ptr<Assets::Asset>>& GetAssets() { return loaded_assets; }

		std::shared_ptr<Luxia::Assets::Asset> GetAssetFromGUID(const GUID& m_guid) { return loaded_assets.contains(m_guid) ? loaded_assets.find(m_guid)->second : nullptr; }
		std::shared_ptr<Luxia::Assets::AssetFile> GetAssetFileFromPath(const std::filesystem::path& rel_path);
		std::shared_ptr<Luxia::Assets::AssetFile> SerializeAssetFile(const std::filesystem::path& metafile_path);
		
		template <typename... Args>
		std::shared_ptr<Luxia::Assets::AssetFile> Import(const std::filesystem::path& rel_path, const std::string ast_name, Args&&... args) {

			if (!rel_path.has_extension()) { return 0; }
			std::filesystem::path full_path = asset_dir.string() + "/" + rel_path.string();

			std::shared_ptr<Luxia::Assets::AssetFile> asset_file = nullptr;

			// Check if there is metafile
			bool has_metafile = false;
			std::filesystem::path metafile_path = full_path;
			metafile_path.replace_extension(".meta");

			if (std::filesystem::exists(metafile_path)) {
				has_metafile = true;
			}


			Luxia::AssetType type = Luxia::AssetType::NoAsset;
			std::string af_ext = full_path.extension().string();
			for (auto& [ext, ext_type] : extensions) {
				if (af_ext == ext) {
					type = ext_type;
					break;
				}
			}

			if (type == Luxia::AssetType::NoAsset) { return 0; }
			else if (type == Luxia::AssetType::Shader) {
				std::shared_ptr<Luxia::Assets::ShaderFile> shader_file = std::make_shared<Luxia::Assets::ShaderFile>(std::forward<Args>(args)...);
				asset_file = shader_file;
			}
			else {
				asset_file = std::make_shared<Luxia::Assets::AssetFile>();
			}

			// If assetfile creation failed, return nullptr
			if (!asset_file->Create(full_path, rel_path, metafile_path, ast_name, type)) { return nullptr; }

			// Set asset_pool entry
			asset_pool[asset_file->guid] = asset_file;

			return asset_file;
		}
		


		template <typename T> // Load an asset from an asset file
		std::shared_ptr<T> CreateAsset(const std::shared_ptr<Luxia::Assets::AssetFile> asset_file) {
			if (asset_file == nullptr) { LX_CORE_ERROR("Asset file is nullptr"); return nullptr; }
			if (!asset_pool.contains(asset_file->guid)) { LX_CORE_ERROR("Asset pool doesnt have {}", (uint64_t)asset_file->guid); return nullptr; }
			
			// Insantiate the assetfile and asset
			std::shared_ptr<T> asset = nullptr;

			// load differently
			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				model->LoadFromFile(asset_file->srcPath);
				asset = model;
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				std::shared_ptr<Luxia::ITexture> texture = Luxia::Platform::Assets::CreateTexture();
				texture->LoadFromFile(asset_file->srcPath);
				asset = texture;
			}
			else if constexpr (std::is_base_of_v<Luxia::IShader, T>) {
				auto shader_file = std::dynamic_pointer_cast<Luxia::Assets::ShaderFile>(asset_file);
				if (!shader_file) {
					LX_CORE_ERROR("Failed to cast AssetFile to ShaderFile");
					return nullptr;
				}

				auto shader = Luxia::Platform::Assets::CreateShader(
					shader_file->fragPath.string().c_str(),
					shader_file->vertPath.string().c_str()
				);

				asset = shader;
			}
			

			loaded_assets[asset->guid] = asset;
			return asset;
		}

		template <typename T> // Load an independant asset with an assigned guid
		std::shared_ptr<T> CreateAsset(const GUID& assigned_guid) {
			std::shared_ptr<T> asset = nullptr;

			// load differently
			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				model->guid = assigned_guid;
				asset = model;
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				std::shared_ptr<Luxia::ITexture> texture = Luxia::Platform::Assets::CreateTexture();
				texture->guid = assigned_guid;
				asset = texture;
			}

			loaded_assets[asset->guid] = asset;
			return asset;
		}

		template <typename T> // Load an independant asset with a new guid
		std::shared_ptr<T> CreateAsset() {
			std::shared_ptr<T> asset = nullptr;
			GUID m_guid;

			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				m_guid = model->guid;
				asset = model;
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				std::shared_ptr<Luxia::ITexture> texture = Luxia::Platform::Assets::CreateTexture();
				m_guid = texture->guid;
				asset = texture;
			}

			loaded_assets[m_guid] = asset;
			return asset;
		}


		template <typename T> // Unload an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, void>
			UnloadAsset(const GUID& guid) {

			if (!loaded_assets.contains(guid)) { return; }

			auto& asset = loaded_assets.find(guid)->second;
			asset->~Asset();

			loaded_assets.erase(guid);
		}

		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<GUID, std::shared_ptr<Assets::Asset>> loaded_assets;
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>> asset_pool;

		std::filesystem::path asset_dir;
	};

};