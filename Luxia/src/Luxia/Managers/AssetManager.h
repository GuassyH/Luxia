#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/TextureFile.h"
#include "Luxia/Asset/ModelFile.h"
#include "Luxia/Asset/SceneFile.h"
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
		int NumAssetPool() { return asset_pool.size(); }
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>>& GetAssetFiles() { return asset_pool; }

		bool LoadAssetPool(const std::filesystem::path& m_path);
		bool SaveAssetPool();
		void Cleanup(); // Before app closes, save everything, etc


		std::shared_ptr<Assets::AssetFile> SerializeAssetFile(std::filesystem::path& metafile_path);
		
		template <typename... Args>
		std::shared_ptr<Assets::AssetFile> Import(const std::filesystem::path& rel_path, const std::string ast_name, Args&&... args) {
			std::filesystem::path full_path = asset_dir / rel_path.lexically_normal();
			if (!std::filesystem::exists(full_path)) { LX_CORE_ERROR("AssetManager: Import, file doesnt exist {}", full_path.string()); return nullptr; }

			std::shared_ptr<Assets::AssetFile> asset_file = nullptr;

			std::filesystem::path metafile_path = full_path;
			metafile_path.replace_extension(".meta");

			AssetType type = AssetType::NoType;
			std::string af_ext = full_path.extension().string();
			auto it = extensions.find(af_ext);
			if (it != extensions.end())
				type = it->second;

			asset_file = MakeSharedFileFromType(type);

			if (!asset_file) { return nullptr; }
			if (!asset_file->Load(metafile_path)) { return nullptr; }


			// Set asset_pool entry
			asset_pool[asset_file->guid] = asset_file;
			asset_SrcToGuid[asset_file->srcPath] = asset_file->guid;

			return asset_file;
		}


		template <typename... Args>
		std::shared_ptr<Assets::AssetFile> CreateAssetFile (const std::filesystem::path& rel_path, const std::string ast_name, const AssetType type, Args&&... args) {
			if (!rel_path.has_extension()) { return nullptr; }
			std::filesystem::path full_path = asset_dir / rel_path.lexically_normal();

			if (std::filesystem::exists(full_path)) { LX_CORE_ERROR("AssetManager: File already exists and cant be created {}", full_path.string()); }
			else {
				std::ofstream new_file(full_path);
				if (!new_file.is_open()) { LX_CORE_ERROR("AssetManager: Failed creating new assetfile"); return nullptr; }
				new_file.close();
			}


			if (std::filesystem::exists(full_path.replace_extension(".meta"))) { LX_CORE_ERROR("AssetManager: File already exists and cant be created {}", full_path.replace_extension(".meta").string()); }
			else {
				std::ofstream new_metafile(full_path.replace_extension(".meta"));
				if (!new_metafile.is_open()) { LX_CORE_ERROR("AssetManager: Failed creating new asset meta-file"); return nullptr; }
				new_metafile.close();
			}


			std::filesystem::path metafile_path = full_path;
			metafile_path.replace_extension(".meta");

			std::shared_ptr<Assets::AssetFile> ast_file = std::make_shared<Luxia::Assets::AssetFile>();
			ast_file->Create(full_path, rel_path, metafile_path, ast_name, type);

			return ast_file;
		}

		
		template <typename T>
		std::shared_ptr<T> GetAssetFileFromPath(const std::filesystem::path& rel_path) {
			std::filesystem::path full_path = asset_dir / rel_path.lexically_normal();

			if (asset_SrcToGuid.contains(full_path)) {
				std::shared_ptr<T> cast_file = std::dynamic_pointer_cast<T>(asset_pool.find(asset_SrcToGuid.find(full_path)->second)->second);
				if (cast_file)
					return cast_file;
			}

			LX_CORE_ERROR("'GetAssetFileFromPath': Asset file nullptr");
			return nullptr;
		}

		template <typename... Args>
		std::shared_ptr<Luxia::Assets::AssetFile> MakeSharedFileFromType(const Luxia::AssetType type, Args&&... args) {

			switch (type)
			{
			case AssetType::ShaderType: 
				return std::make_shared<Luxia::Assets::ShaderFile>(std::forward<Args>(args)...);
			case AssetType::TextureType: 
				return std::make_shared<Luxia::Assets::TextureFile>();
			case AssetType::ModelType:
				return std::make_shared<Luxia::Assets::ModelFile>();
			case AssetType::AudioType:
				return nullptr;
			case AssetType::MaterialType:
				return nullptr;
			case AssetType::SceneType:
				return std::make_shared<Luxia::Assets::SceneFile>();;
			default:
				LX_CORE_ERROR("Make Shared from type failed, unknown type");
				return nullptr;
			}
		}

		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>> asset_pool;
		std::unordered_map<std::filesystem::path, GUID> asset_SrcToGuid;
		std::filesystem::path asset_dir;
	};

};