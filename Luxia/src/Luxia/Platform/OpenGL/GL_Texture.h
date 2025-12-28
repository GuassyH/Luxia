#pragma once

#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia::Platform::OpenGL {
	class LUXIA_API GL_Texture : public ITexture {
	public:
		GL_Texture() : ITexture() {}
		~GL_Texture() = default;

		virtual void CreateFBOTex(const int width, const int height) override;
		virtual void CreateEmpty(const int width, const int height, const int colCh) override;
		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool m_flip = false) override;
		virtual bool Unload() override { return true; }
		virtual void Delete() override;
		virtual void Use() override;

	};
}