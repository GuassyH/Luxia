#pragma once


#include "Luxia/Core.h"
#include "Asset.h"
#include <vector>

namespace Luxia::Assets {

	class LUXIA_API ModelAsset : public Asset {
	public:
		ModelAsset() {
			type = AssetType::Model;
			loaded = false;
		}
		virtual void Load(std::string& m_path) override {
			path = m_path;


			bool suffix_found = false;
			if (path.find(".") != std::string::npos) {
				int suf_start = path.find_last_of(".");
				std::string m_suf = path.substr(suf_start, path.size() - suf_start);

				for (auto& t_suf : p_suf) {
					if (t_suf == m_suf) {
						suffix = m_suf;
						suffix_found = true;
						break;
					}
				}

				std::string name_path = path.substr(0, suf_start);

				if (name_path.find_last_of("/") != std::string::npos) {
					int name_start = name_path.find_last_of("/") + 1;
					std::string m_name = name_path.substr(name_start, name_path.size() - name_start);
					name = m_name;
				}

			}

			loaded = suffix_found;
			LX_CORE_TRACE("Loaded Model Asset: '{}', {}", name, suffix);
		}
		virtual void Unload() override {}
	private:
		std::array<std::string, 4> p_suf{
			".obj",
			".fbx",
			".gltf"
		};
	};
}