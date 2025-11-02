#pragma once

#include "ITexture.h"

namespace Luxia::Platform {
	class Win_Texture : public ITexture {
	public:
		Win_Texture() = default;
		~Win_Texture() = default;

		virtual void LoadFromFile(const std::filesystem::path& m_path, const bool flip = false) override;
		virtual void Unload() override;
		virtual void Use() override;

		virtual void Delete() override;
	};
}