#pragma once


#include "Luxia/Core.h"
#include "Asset.h"
#include <vector>

// using Luxia::Model

namespace Luxia::Assets {

	class LUXIA_API ModelAsset : public Asset {
	public:
		// Luxia::Model model = Luxia::Model();

		ModelAsset() {
			type = AssetType::Model;
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

			loaded = suffix_found;
			LX_CORE_TRACE("{} Model Asset: '{}', {}", loaded ? "Loaded" : "Not Loaded", name, suffix);
		}
		virtual void Unload() override {}
	private:
		std::array<std::string, 3> p_suf{
			".obj",
			".fbx",
			".gltf"
		};
	};
}