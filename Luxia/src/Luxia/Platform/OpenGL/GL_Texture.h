#pragma once

#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Texture : public ITexture {
	public:
		GL_Texture() = default;
		~GL_Texture() = default;

		virtual void CreateEmpty(const int width, const int height, const int colCh) override;
		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) override;
		virtual void Delete() override;
		virtual void Use() override;
	};
}