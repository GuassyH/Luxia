#pragma once
#include "AssetFile.h"

#include "Luxia/Core/Core.h"

namespace Luxia::Assets {

	class LUXIA_API ModelAsset : public AssetFile {
	public:

		ModelAsset() {
			type = AssetType::Model;
			loaded = false;
		}

		virtual void Create(const std::filesystem::path& sourcePath) override {
			srcPath = sourcePath;

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

			// Create metafile, save data
			loaded = suffix_found && srcPath.has_filename() && srcPath.has_extension();

			if (loaded)
				LX_CORE_TRACE("Created Model Asset: '{}', {}", name, suffix);
			else
				LX_CORE_ERROR("Failed Created Model Asset: {}", srcPath.string());
		}

		virtual void Load(const std::filesystem::path& metaFile) {
			// Get metafile, parse data
		}


		virtual void Unload() override {
		
		}
	private:
		std::array<std::string, 3> p_suf{
			".obj",
			".fbx",
			".gltf"
		};
	};
}