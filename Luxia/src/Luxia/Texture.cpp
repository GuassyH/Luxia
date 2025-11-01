#include "Texture.h"


namespace Luxia {

	void Texture::LoadFromFile(const std::filesystem::path& m_path, const bool flip) {
		// Load Texture
		if (flip) { stbi_set_flip_vertically_on_load(true); }

		// Read texture file

		// Reset flip vertically on load since the next might not want to
		stbi_set_flip_vertically_on_load(false);
		valid = true;
	}

	void Texture::Unload() {
		// Unload Texture
		valid = false;
	}

	void Texture::Use() {
		if (!IsValid()) { LX_CORE_ERROR("Tried to use unvalid texture: {}", path); }
		
		// Bind currently used texture to be this
	}

	void Texture::Delete() {
		// Delete Texture
	}
};