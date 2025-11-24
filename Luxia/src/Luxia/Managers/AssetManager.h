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


		std::shared_ptr<Luxia::Assets::AssetFile> SerializeAssetFile(std::filesystem::path& metafile_path);
		
		template <typename... Args>
		std::shared_ptr<Luxia::Assets::AssetFile> Import(const std::filesystem::path& rel_path, const std::string ast_name, Args&&... args) {
			if (!rel_path.has_extension()) { return nullptr; }
			std::filesystem::path full_path = asset_dir / rel_path.lexically_normal();

			std::shared_ptr<Luxia::Assets::AssetFile> asset_file = nullptr;

			std::filesystem::path metafile_path = full_path;
			metafile_path.replace_extension(".meta");

			Luxia::AssetType type = Luxia::AssetType::NoAsset;
			std::string af_ext = full_path.extension().string();
			auto it = extensions.find(af_ext);
			if (it != extensions.end())
				type = it->second;

			asset_file = MakeSharedFileFromType(type);

			if (!asset_file) { return nullptr; }
			if (!asset_file->Create(full_path, rel_path, metafile_path, ast_name, type)) { return nullptr; }

			// Set asset_pool entry
			asset_pool[asset_file->guid] = asset_file;
			asset_SrcToGuid[asset_file->srcPath] = asset_file->guid;

			return asset_file;
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
			case Luxia::AssetType::Shader: 
				return std::make_shared<Luxia::Assets::ShaderFile>(std::forward<Args>(args)...);
			case Luxia::AssetType::Texture: 
				return std::make_shared<Luxia::Assets::TextureFile>();
			case Luxia::AssetType::Model:
				return std::make_shared<Luxia::Assets::ModelFile>();
			case Luxia::AssetType::Audio:
				return nullptr;
				// return std::make_shared<Luxia::Assets::ModelFile>();
			case Luxia::AssetType::Material:
				return nullptr;
				// return std::make_shared<Luxia::Assets::ModelFile>();
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