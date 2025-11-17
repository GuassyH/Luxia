#include "lxpch.h"
#include "AssetFile.h"


namespace Luxia::Assets {
	void AssetFile::Create(const std::filesystem::path& sourcePath, const AssetType& m_type) {
		type = type;
		srcPath = sourcePath;
		extension = sourcePath.extension().string();
		name = sourcePath.filename().string();
		
		loaded = true;
	}
	void AssetFile::Load(const std::filesystem::path& metaPath){
		// load metafile
	}
	void AssetFile::Unload(){
	}
}