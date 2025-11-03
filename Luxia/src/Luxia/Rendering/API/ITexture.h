#pragma once

#include "Luxia/Core.h"
#include <filesystem>
#include "stb/stb_image.h"
#include "Luxia/Log.h"

namespace Luxia {
	class LUXIA_API ITexture {
	public:
		unsigned int texID = 0;

		ITexture() = default;
		virtual ~ITexture() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) = 0;
		virtual void Unload() = 0;
		virtual void Use() = 0;

		virtual void Delete() = 0;

		bool IsValid() { return valid; }
		int GetWidth() { return imgWidth; }
		int GetHeight() { return imgHeight; }
		int GetNumColCh() { return numColCh; }
	protected:
		int imgWidth = 0, imgHeight = 0, numColCh = 3;
		std::string path;

		bool valid = false;
	};
}