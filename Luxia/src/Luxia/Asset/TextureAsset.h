#pragma once

#include "Luxia/Core/Core.h"

#include "AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API TextureAsset : public AssetFile {
	public:

		TextureAsset() { 
			type = AssetType::Texture; 
			loaded = false;
		}

		virtual void Create(const std::filesystem::path& sourcePath) override {
			srcPath = sourcePath;

			loaded = true;

			if (loaded)
				LX_CORE_TRACE("Created Model Asset: '{}'", name);
			else
				LX_CORE_ERROR("Failed Created Model Asset: {}", srcPath.string());
		}


		virtual void Load(const std::filesystem::path& metaFile) {


			guid = 0; // EQUALS METAFILE
		}


		virtual void Unload() override {
		
		}


	private:
	};
}