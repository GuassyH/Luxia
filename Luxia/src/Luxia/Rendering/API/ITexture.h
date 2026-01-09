#pragma once

#include "Luxia/Core/Core.h"
#include "stb/stb_image.h"

#include "Luxia/Asset/Asset.h"

#include <filesystem>

namespace Luxia {
	enum LUXIA_API TextureType {
		LX_TEXTURE_DIFFUSE		= 0,
		LX_TEXTURE_SPECULAR		= 1,
		LX_TEXTURE_NORMALS		= 2,
		LX_TEXTURE_ROUGHNESS	= 3,
		LX_TEXTURE_FBO			= 4
	};

	class LUXIA_API ITexture : public Luxia::Assets::Asset {
	public:
		unsigned int texID = 0;

		ITexture() { type = Luxia::AssetType::TextureType; }
		virtual ~ITexture() = default;

		virtual void CreateFBOTex(const int width, const int height) = 0;
		virtual void CreateDepthTex(const int width, const int height) = 0;
		virtual void CreateEmpty(const int width, const int height, const int colCh) = 0; // For custom textures created through code, such as FBO tex
		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool m_flip = false) = 0; // Loading textures
		virtual bool Unload() override = 0;
		virtual void Delete() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		bool IsValid() const	{ return valid; }
		bool HasPath() const	{ return hasPath; }
		bool GetFlip() const	{ return flip; }
		int GetWidth() const	{ return imgWidth; }
		int GetHeight() const	{ return imgHeight; }
		int GetNumColCh() const	{ return numColCh; }

		unsigned int GetFBO() const { return fbo; }
		unsigned int GetRBO() const { return rbo; }
		bool IsFBOTex() const { return is_fbo_tex; }

		TextureType tex_type = TextureType::LX_TEXTURE_DIFFUSE;
		std::filesystem::path path = "";
	protected:
		int imgWidth = 0, imgHeight = 0, numColCh = 3;
		unsigned int fbo = 0, rbo = 0;
		bool hasPath = false;
		bool valid = false;
		bool is_fbo_tex = false;
		bool flip = false;
	};
}