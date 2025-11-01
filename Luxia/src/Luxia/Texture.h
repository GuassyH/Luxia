#pragma once

#include "Luxia/Core.h"
#include "Luxia/Log.h"
#include <iostream>
#include "stb/stb_image.h"

#include <filesystem>

namespace Luxia {
	class LUXIA_API Texture {
	public:
		unsigned int texID = 0;

		Texture() = default;
		~Texture() = default;

		void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false);
		void Unload();
		void Use();

		void Delete();

		bool IsValid() { return valid; }
	private:
		int imgWidth, imgHeight, numColCh = 0;
		std::string path;

		bool valid = false;
	};
}