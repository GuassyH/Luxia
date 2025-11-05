#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Platform/AssetCreation.h"

#include "Asset.h"

using Luxia::ITexture;

namespace Luxia::Assets {
	class LUXIA_API TextureAsset : public Asset {
	public:
		std::shared_ptr<Luxia::ITexture> texture = nullptr;

		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
		}

		virtual void Create(const std::filesystem::path& sourcePath) override {
			srcPath = sourcePath;

			// Create OS specific Texture
			texture = Luxia::Platform::Assets::CreateTexture();

			bool suffix_found = false;
			if (srcPath.has_extension()) {
				std::string m_suf = srcPath.extension().string();

				for (auto& t_suf : p_suf) {
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}

				std::string name_path = srcPath.stem().string();
				name = name_path;
			}

			if (suffix_found) {
				texture->LoadFromFile(srcPath);
			}
			
			loaded = texture->IsValid();	

			if (loaded)
				LX_CORE_TRACE("Created Texture Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Created Texture Asset: {}", srcPath.string());
		}


		virtual void Load(const std::filesystem::path& metaFile) {

		}


		virtual void Unload() override {
			texture->Delete();
		}


	private:
		std::array<std::string, 3> p_suf{
			".png",
			".jpg",
			".bmp"
		};
	};
}