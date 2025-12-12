#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Components/Transform.h"

#include "Luxia/Platform/PlatformDefinitions.h"
#include "Luxia/Entity.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/TextureFile.h"
#include "Luxia/Asset/ModelFile.h"
#include "Luxia/Asset/Asset.h"

#include "Luxia/Core/GUID.h"
#include "entt/entt.hpp"
#include "Luxia/Asset/SceneFile.h"

namespace Luxia {

	class LUXIA_API Scene
	{
	public:
		Scene() : reg(entt::registry()) {}
		~Scene() = default;

		void Cleanup() { reg.clear(); }
		void Load() { reg = entt::registry(); }
		
		void SaveToFile(const std::shared_ptr<Luxia::Assets::SceneFile> scene_file);
		void LoadFromFile(const std::shared_ptr<Luxia::Assets::SceneFile> scene_file);

		Entity& CreateEntity(std::string name = "Entity");
		
		// Copied and modified entt/entt.hpp view func to forward
		template<typename Type, typename... Other, typename... Exclude>
		entt::basic_view<entt::get_t<entt::storage_for_t<Type>, entt::storage_for_t<Other>...>, entt::exclude_t<entt::storage_for_t<Exclude>...>>
			GetEntitiesWith(entt::exclude_t<Exclude...> = entt::exclude_t{}) {
			return reg.view<Type>();
		}

		template<typename T>
		T* TryGetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.try_get<T>(entity);
		}

		template<typename T>
		T& GetFromEntity(const entt::entity entity) { // Only implemented to make it more readable to me
			return reg.get<T>(entity);
		}

		entt::registry& GetReg() { return reg; }


		size_t NumLoaded() { return loaded_assets.size(); }
		const std::unordered_map<GUID, std::shared_ptr<Assets::Asset>>& GetAssets() { return loaded_assets; }
		std::shared_ptr<Assets::Asset> GetAssetFromGUID(const GUID& m_guid) { return loaded_assets.contains(m_guid) ? loaded_assets.find(m_guid)->second : nullptr; }

		/// Might rethink this structure
		template <typename T> // Load an asset from an asset file
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, std::shared_ptr<T>> 
			LoadRuntimeAsset(const std::shared_ptr<Luxia::Assets::AssetFile> asset_file) {
			if (asset_file == nullptr) { LX_CORE_ERROR("Asset file is nullptr"); return nullptr; }

			// Insantiate the assetfile and asset
			std::shared_ptr<T> asset = nullptr;

			// load differently
			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				std::shared_ptr<Luxia::IModel> model = Luxia::Platform::Assets::CreateModel();
				std::shared_ptr<Luxia::Assets::ModelFile> model_file = std::dynamic_pointer_cast<Luxia::Assets::ModelFile>(asset_file);
				if (model_file) {
					model->LoadFromFile(model_file, this);
				}
				asset = model;
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				std::shared_ptr<Luxia::ITexture> texture = Luxia::Platform::Assets::CreateTexture();
				std::shared_ptr<Luxia::Assets::TextureFile> texture_file = std::dynamic_pointer_cast<Luxia::Assets::TextureFile>(asset_file);
				if (texture_file) {
					texture->LoadFromFile(texture_file->texture_path);
				}
				asset = texture;
			}
			else if constexpr (std::is_base_of_v<Luxia::IShader, T>) {
				auto shader_file = std::dynamic_pointer_cast<Luxia::Assets::ShaderFile>(asset_file);
				if (!shader_file) {
					LX_CORE_ERROR("Failed to cast AssetFile to ShaderFile");
					return nullptr;
				}
				std::shared_ptr<Luxia::IShader> shader = nullptr;
				if (shader_file->shaderType == Luxia::Assets::ShaderFile::ShaderFileType::VertexFragment) {
					shader = Luxia::Platform::Assets::CreateShader(
						shader_file->fragPath.string().c_str(),
						shader_file->vertPath.string().c_str()
					);
				}
				asset = shader;
			}

			asset->guid = GUID();
			loaded_assets[asset->guid] = asset;
			return asset;
		}

		template <typename T, typename... Args> // Load an independant asset with a new guid
		std::enable_if_t<std::is_base_of_v<Luxia::Assets::Asset, T>, std::shared_ptr<T>>  
			CreateRuntimeAsset() {
			std::shared_ptr<T> asset = nullptr;

			// load differently
			if constexpr (std::is_base_of_v<Luxia::IModel, T>) {
				asset = Luxia::Platform::Assets::CreateModel();
			}
			else if constexpr (std::is_base_of_v<Luxia::ITexture, T>) {
				asset = Luxia::Platform::Assets::CreateTexture();
			}
			else if constexpr (std::is_base_of_v<Luxia::IShader, T>) {
				asset = Luxia::Platform::Assets::CreateShader();
			}

			asset->guid = GUID();
			loaded_assets[asset->guid] = asset;
			return asset;
		}

		template <typename T> // Unload an asset from a path
		std::enable_if_t<std::is_base_of_v<Assets::Asset, T>, bool>
			UnloadAsset(const GUID& guid) {
			if (!loaded_assets.contains(guid)) { return false; }

			auto& asset = loaded_assets.find(guid)->second;
			asset->Unload();

			loaded_assets.erase(guid);
			return true;
		}

		std::unordered_map<GUID, Entity> runtime_entities;
	private:
		friend class SceneSerializer;
		entt::registry reg;
		std::unordered_map<GUID, std::shared_ptr<Assets::Asset>> loaded_assets;
	};
}

