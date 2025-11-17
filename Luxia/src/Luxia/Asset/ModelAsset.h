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

			loaded = true;

			if (loaded)
				LX_CORE_TRACE("Created Model Asset: '{}'", name);
			else
				LX_CORE_ERROR("Failed Created Model Asset: {}", srcPath.string());
		}

		virtual void Load(const std::filesystem::path& metaFile) {
			// Get metafile, parse data
		}


		virtual void Unload() override {
		
		}
	private:
	};
}