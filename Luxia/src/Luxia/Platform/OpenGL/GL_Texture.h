#pragma once

#include "Luxia/Rendering/API/ITexture.h"

namespace Luxia::Platform {
	class GL_Texture : public ITexture {
	public:
		GL_Texture() = default;
		~GL_Texture() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) override;
		virtual void Unload() override;
		virtual void Use() override;

		virtual void Delete() override;
	};
}