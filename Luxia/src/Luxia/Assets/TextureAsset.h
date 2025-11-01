#pragma once

#include "Luxia/Core.h"
#include "Luxia/Texture.h"

#include "Asset.h"
#include <vector>

using Luxia::Texture;

namespace Luxia::Assets {
	class LUXIA_API TextureAsset : public Asset {
	public:
		Luxia::Texture texture = Luxia::Texture();

		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
		}
		virtual void Load(const std::filesystem::path& m_path) override { 
			path = m_path; 

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
			}

			texture.LoadFromFile(path);

			loaded = texture.IsValid();	
			
			if(loaded)
				LX_CORE_TRACE("{} Texture Asset: '{}', {}", loaded ? "Loaded" : "Not Loaded", name, suffix);
			else
				LX_CORE_ERROR("{} Texture Asset: '{}', {}", loaded ? "Loaded" : "Not Loaded", name, suffix);

		}
		virtual void Unload() override {
			texture.Unload();
		}

		virtual void Delete() override {
			Unload();
			texture.Delete();
		}

	private:
		std::array<std::string, 3> p_suf{
			".png",
			".jpg",
			".bmp"
		};
	};
}