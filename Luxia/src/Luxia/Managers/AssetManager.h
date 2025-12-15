#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/TextureFile.h"
#include "Luxia/Asset/MaterialFile.h"
#include "Luxia/Asset/ModelFile.h"
#include "Luxia/Asset/SceneFile.h"
#include "Luxia/Asset/MetaFile.h"


#include "Luxia/Platform/PlatformDefinitions.h"

#include <unordered_map>
#include <iostream>
#include <vector>
#include <xutility>

namespace Luxia {


	class LUXIA_API AssetManager {
	private:
		static std::unordered_map<std::string, Luxia::AssetType> extensions;
		static std::unordered_map<Luxia::AssetType, std::string> asset_extensions;
	public:
		bool LoadAssetPool(const std::filesystem::path& m_path);
		bool SaveAssetPool(const std::filesystem::path& m_path);
		void Cleanup(); // Before app closes, save everything, etc

	#pragma region Files

		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>>& GetAssetFilePool() { return assetfile_pool; }
		std::unordered_map<GUID, std::shared_ptr<Assets::MetaFile>>& GetMetaFilePool() { return meta_pool; }


		GUID GetAssetFileGUID(const std::filesystem::path& rel_path) {
			std::filesystem::path abs_path = asset_dir / rel_path.lexically_normal();
			// Search meta pool for matching srcPath
			for (auto& [guid, meta_file] : meta_pool) {
				if (meta_file->assetPath == abs_path) {
					return guid;
				}
			}
			LX_CORE_ERROR("Asset Manager: GetAssetFileGUID - No matching GUID found for path {}", abs_path.string());
			return GUID(0);
		}

		// Casts the file to the requested type
		template <typename T>
		std::shared_ptr<T> GetAssetFile(const GUID& guid) {
			if (!assetfile_pool.contains(guid)) {
				LX_CORE_ERROR("Asset Manager: GetAssetFile - GUID not found {}", static_cast<uint64_t>(guid));
				return nullptr;
			}
			std::shared_ptr<Assets::AssetFile> asset_file = assetfile_pool.find(guid)->second;
			if (!asset_file) {
				LX_CORE_ERROR("Asset Manager: GetAssetFile - AssetFile is nullptr for GUID {}", static_cast<uint64_t>(guid));
				return nullptr;
			}
			// Dynamic cast to the requested type
			std::shared_ptr<T> casted_asset_file = std::dynamic_pointer_cast<T>(asset_file);
			if (!casted_asset_file) {
				LX_CORE_ERROR("Asset Manager: GetAssetFile - Dynamic cast failed for GUID {}", static_cast<uint64_t>(guid));
				return nullptr;
			}
			LX_CORE_INFO("Asset Manager: GetAssetFile - Successfully retrieved asset file for GUID {}", static_cast<uint64_t>(guid));
			return casted_asset_file;
		}

		// Creates an asset file of the given type at the given relative path with the given name
		template <Luxia::AssetType type, typename... Args> // where_rel_path is where it should be created 
		GUID CreateAssetFile(const std::filesystem::path& where_rel_path, const std::string ast_name, Args&&... args) 
		{
			if (type == AssetType::NoType) { LX_CORE_ERROR("Asset Manager: CreateAssetFile - NoType given"); return GUID(0); }
			std::string suf = asset_extensions.find(type)->second;

			// Get the absolute path
			std::filesystem::path abs_path = asset_dir / where_rel_path.lexically_normal() / ast_name;

			LX_CORE_TRACE("Asset Manager: CreateAssetFile - Creating asset at path {}", abs_path.string());

			// Create paths 
			std::filesystem::path assetfile_path = abs_path.replace_extension(suf);
			std::filesystem::path metafile_path = abs_path.replace_extension(".meta");

			if (std::filesystem::exists(abs_path) || std::filesystem::exists(metafile_path) || std::filesystem::exists(assetfile_path)) {
				LX_CORE_ERROR("Asset Manager: Cannot CreateAssetFile since one of the paths exists");
				return GUID(0);
			}

			std::shared_ptr<Assets::AssetFile> asset_file = NewAssetFile(assetfile_path, type, std::forward<Args>(args)...);
			std::shared_ptr<Assets::MetaFile> meta_file = NewMetaFile(metafile_path, assetfile_path, ast_name, type);

			// Check both created
			if (!asset_file || !meta_file) {
				LX_CORE_ERROR("Asset Manager: Importing asset failed for - {}", abs_path.string());
				return GUID(0);
			}

			// Assign both files pools
			assetfile_pool[meta_file->guid] = asset_file;
			meta_pool[meta_file->guid] = meta_file;

			// Return guid
			return meta_file->guid;
		}
		
		// Import, used for models (.gltf etc) and textures (.png, .jpg etc)
		template <typename... Args>
		GUID Import(const std::filesystem::path& rel_path, const std::string ast_name, Args&&... args) {
			std::filesystem::path abs_path = asset_dir / rel_path.lexically_normal();

			if (!std::filesystem::exists(abs_path)) { LX_CORE_ERROR("AssetManager: Import - Cannot import non existent file"); return GUID(0); }

			// Get type from extension
			std::filesystem::path assetfile_path = abs_path;
			std::filesystem::path metafile_path = abs_path;

			AssetType type = AssetType::NoType;
			std::string typesuf = "";
			for (auto& [a_suf, atype] : extensions) {
				if (abs_path.string().find(a_suf, abs_path.string().size() - a_suf.size()) != std::string::npos) {
					type = atype;
					typesuf = asset_extensions.find(type)->second;
					break;
				}
			}
			if (type == AssetType::NoType || typesuf == "") {
				LX_CORE_ERROR("Asset Manager: Importing asset failed for - {} (unknown extension)", rel_path.string());
				return GUID(0);
			}

			assetfile_path.replace_extension(typesuf);
			metafile_path += ".meta";

			// Create both files
			std::shared_ptr<Assets::AssetFile> asset_file = NewAssetFile(assetfile_path, type, std::forward<Args>(args)...);
			std::shared_ptr<Assets::MetaFile> meta_file = NewMetaFile(metafile_path, assetfile_path, ast_name, type);

			// Check both created
			if(!asset_file || !meta_file) {
				LX_CORE_ERROR("Asset Manager: Importing asset failed for - {}", rel_path.string());
				return GUID(0);
			}


			asset_file->guid = meta_file->guid;

			// Assign both files pools
			assetfile_pool[meta_file->guid] = asset_file;
			meta_pool[meta_file->guid] = meta_file;

			// Return guid
			return meta_file->guid;
		}


		template <typename... Args>
		std::shared_ptr<Assets::AssetFile> NewAssetFile(const std::filesystem::path& assetfile_path, const AssetType type, Args&&... args) {
			if (std::filesystem::exists(assetfile_path)) {
				LX_CORE_ERROR("Asset Manager: NewAssetFile failed, asset path exists - {}", assetfile_path.string());
				return nullptr;
			}

			// Create assetfile
			std::shared_ptr<Assets::AssetFile> asset_file = MakeAssetFileFromType(type, std::forward<Args>(args)...);
			LX_CORE_TRACE("Asset File path - {}", assetfile_path.string());
			
			if (!asset_file->Create(assetfile_path)) {
				LX_CORE_ERROR("Asset Manager: NewAssetFile failed to create metafile for - {}", assetfile_path.string());
				return nullptr;
			}

			for(auto asset : asset_file->assets){
				if (asset_pool.contains(asset->guid)) continue;

				asset_pool[asset->guid] = asset.lock();
			}

			// Return it
			return asset_file;
		}

		std::shared_ptr<Assets::MetaFile> NewMetaFile(std::filesystem::path& metafile_path, std::filesystem::path& assetfile_path, const std::string& name, const AssetType type) {
			// Create metafile
			std::shared_ptr<Assets::MetaFile> meta_file = std::make_shared<Assets::MetaFile>();
			if(!meta_file->Create(assetfile_path, metafile_path, name, type)) {
				LX_CORE_ERROR("Asset Manager: NewMetaFile failed to create metafile for - {}", metafile_path.string());
				return nullptr;
			}

			// Return it
			return meta_file;
		}

		std::shared_ptr<Assets::AssetFile> LoadAssetFile(const std::filesystem::path& abs_path, const AssetType type) {
			// Load metafile
			// Creates an EMPTY asset file of the given type, no "Args&&... args"
			std::shared_ptr<Assets::AssetFile> asset_file = MakeAssetFileFromType(type);
			if (asset_file->Load(abs_path).size() == 0) {
				LX_CORE_ERROR("Asset Manager: LoadAssetFile failed to load assetfile for - {}", abs_path.string());
				return nullptr;
			}
			for (auto asset : asset_file->assets) {
				if (asset) {
					if (asset_pool.contains(asset->guid)) continue;
					asset_pool[asset->guid] = asset;
				}
			}

			return asset_file;
		}

		std::shared_ptr<Assets::MetaFile> LoadMetaFile(const std::filesystem::path& meta_path) {
			// Load metafile
			std::shared_ptr<Assets::MetaFile> meta_file = std::make_shared<Assets::MetaFile>();
			if(!meta_file->Load(meta_path)) {
				LX_CORE_ERROR("Asset Manager: LoadMetaFile failed to load metafile for - {}", meta_path.string());
				return nullptr;
			}
			
			return meta_file;
		}

		template <typename... Args>
		std::shared_ptr<Luxia::Assets::AssetFile> MakeAssetFileFromType(const Luxia::AssetType type, Args&&... args)
		{
			switch (type)
			{
				case AssetType::NoType:
					LX_CORE_ERROR("Make Shared from type failed, NoType given");
					return nullptr;
				case AssetType::MaterialType:
					return std::make_shared<Assets::MaterialFile>(std::forward<Args>(args)...);
				case AssetType::AudioType:
					LX_CORE_ERROR("Make Shared from type failed, AudioType not implemented");
					return nullptr;
				case AssetType::ModelType:
					return std::make_shared<Assets::ModelFile>(std::forward<Args>(args)...);
				case AssetType::TextureType:
					return std::make_shared<Assets::TextureFile>(std::forward<Args>(args)...);
				case AssetType::SceneType:
					return std::make_shared<Assets::SceneFile>(std::forward<Args>(args)...);;
				case AssetType::ShaderType:
					return std::make_shared<Assets::ShaderFile>(std::forward<Args>(args)...);;
				default:
					return nullptr;
			}

		}

	#pragma endregion

	#pragma region Runtime Assets

		template <typename T>
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, std::shared_ptr<Assets::Asset>>
			GetAsset(GUID guid) {
			std::shared_ptr<T> asset = nullptr;
			if (asset_pool.contains(guid)) {
				asset = std::dynamic_pointer_cast<T>(asset_pool.find(guid)->second);
			}
			return asset;
		}

	#pragma endregion


		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>> assetfile_pool;	// The asset files
		std::unordered_map<GUID, std::shared_ptr<Assets::MetaFile>> meta_pool;			// The meta files
		std::unordered_map<GUID, std::shared_ptr<Assets::Asset>> asset_pool;			// The actual runtime assets
		std::filesystem::path asset_dir;
	};

};