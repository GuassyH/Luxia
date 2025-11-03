#pragma once

#include "Luxia/Core.h"
#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Platform/AssetCreation.h"

#include "Asset.h"
#include <vector>

using Luxia::ITexture;

namespace Luxia::Assets {
	class LUXIA_API TextureAsset : public Asset {
	public:
		std::shared_ptr<Luxia::ITexture> texture = nullptr;

		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
		}
		virtual void Load(const std::filesystem::path& m_path) override { 
			path = m_path; 

			// Create OS specific Texture
			texture = Luxia::Platform::Assets::CreateTexture();

			bool suffix_found = false;
			if (path.has_extension()) {
				std::string m_suf = path.extension().string();

				for (auto& t_suf : p_suf) {
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}

				std::string name_path = path.stem().string();
				name = name_path;
			}

			if (suffix_found) {
				texture->LoadFromFile(path);
			}
			
			loaded = texture->IsValid();	

			if (loaded)
				LX_CORE_TRACE("Loaded Texture Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Loading Texture Asset: {}", path.string());
		}
		virtual void Unload() override {
			texture->Unload();
		}

		virtual void Delete() override {
			Unload();
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