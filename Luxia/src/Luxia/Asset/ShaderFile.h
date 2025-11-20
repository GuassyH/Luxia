#pragma once

#include "Luxia/Asset/AssetFile.h"

namespace Luxia::Assets {
	class LUXIA_API ShaderFile : public AssetFile {
	public:
		ShaderFile() = default;
		ShaderFile(const std::filesystem::path& frag_path, const std::filesystem::path& vert_path) 
			: fragPath(frag_path), vertPath(vert_path) { }
		virtual bool LoadExtra(std::ifstream& m_infile) override;
		virtual bool SaveExtra(std::ofstream& m_outfile) override;

		std::filesystem::path fragPath;
		std::filesystem::path vertPath;
	};
}