#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/Log.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/TextureFile.h"
#include "Luxia/Asset/ModelFile.h"
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

		bool LoadAssetPoolFromPath(const std::filesystem::path& m_path);
		bool SaveAssetPool(const std::filesystem::path& m_path);
		void Cleanup(); // Before app closes, save everything, etc


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
		
		template <typename T>
		std::shared_ptr<T> GetAssetFileFromPath(const std::filesystem::path& rel_path) {
			std::filesystem::path full_path = asset_dir.string() + "/" + rel_path.string();

			for (auto& [t_guid, ast_file] : asset_pool) {
				if (ast_file->srcPath == full_path) {
					std::shared_ptr<T> cast_file = std::dynamic_pointer_cast<T>(asset_pool.find(t_guid)->second);
					if (cast_file) {
						return cast_file;
					}
					else {
						LX_CORE_TRACE("GetAssetFileFromPath: Asset file nullptr");
					}
				}
			}

			return nullptr;
		}



		~AssetManager() = default;
		AssetManager() = default;
	private:
		std::unordered_map<GUID, std::shared_ptr<Assets::AssetFile>> asset_pool;
		std::filesystem::path asset_dir;
	};

};