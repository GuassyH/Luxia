#pragma once

#include "Luxia/Core/Core.h"

#include "Asset.h"

namespace Luxia::Assets {
	class LUXIA_API TextureAsset : public Asset {
	public:

		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
		}

		virtual void Create(const std::filesystem::path& sourcePath) override {
			srcPath = sourcePath;

			// Create OS specific Texture

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

			loaded = true;

			if (loaded)
				LX_CORE_TRACE("Created Texture Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Created Texture Asset: {}", srcPath.string());
		}


		virtual void Load(const std::filesystem::path& metaFile) {

		}


		virtual void Unload() override {
		
		}


	private:
		std::array<std::string, 3> p_suf{
			".png",
			".jpg",
			".bmp"
		};
	};
}