#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Components/Transform.h"

#include "Luxia/Platform/PlatformDefinitions.h"

#include "Luxia/Asset/AssetFile.h"
#include "Luxia/Asset/ShaderFile.h"
#include "Luxia/Asset/TextureFile.h"
#include "Luxia/Asset/ModelFile.h"
#include "Luxia/Asset/Asset.h"

#include "Luxia/Core/GUID.h"
#include "entt/entt.hpp"

namespace Luxia {

	class LUXIA_API Scene
	{
	public:
		Scene() : reg(entt::registry()) {}
		~Scene() = default;

		void Cleanup() { reg.clear(); }
		void Load() { reg = entt::registry(); }
		void LoadFromFile(const std::filesystem::path& m_metaPath);

		Components::Transform& CreateEntity();
		
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


		int NumLoaded() { return loaded_assets.size(); }
		const std::unordered_map<GUID, std::shared_ptr<Assets::Asset>>& GetAssets() { return loaded_assets; }
		std::shared_ptr<Assets::Asset> GetAssetFromGUID(const GUID& m_guid) { return loaded_assets.contains(m_guid) ? loaded_assets.find(m_guid)->second : nullptr; }


		template <typename T> // Load an asset from an asset file
		std::shared_ptr<T> CreateAsset(const std::shared_ptr<Luxia::Assets::AssetFile> asset_file) {
			if (asset_file == nullptr) { LX_CORE_ERROR("Asset file is nullptr"); return nullptr; }

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
	private:
		entt::registry reg;
		std::unordered_map<GUID, std::shared_ptr<Assets::Asset>> loaded_assets;
	};
}

