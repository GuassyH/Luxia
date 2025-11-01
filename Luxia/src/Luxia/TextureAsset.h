#pragma once

#include "Luxia/Core.h"
#include "Asset.h"
#include <vector>

namespace Luxia {
	
	// Possible suffixes
	inline std::array<std::string, 4> p_suf{
		".png",
		".jpg",
		".JPG",
		".bmp"
	};

	class LUXIA_API TextureAsset : public Asset {
	public:
		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
			LX_CORE_WARN("Instantiated Texture Asset");
		}
		virtual void Load(std::string& m_path) override { 
			path = m_path; 
			
			LX_CORE_WARN("Loaded Texture Asset at: {}", path.c_str());

			bool suffix_found = false;
			if (path.find(".") != std::string::npos) {
				int s = path.find_last_of(".");
				std::string m_suf = path.substr(s, path.size() - s);
				
				for (auto t_suf : p_suf)	{
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}
			}


			loaded = suffix_found;
		}
		virtual void Unload() override {}
	};
}