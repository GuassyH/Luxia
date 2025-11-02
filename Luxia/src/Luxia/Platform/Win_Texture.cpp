#include "Win_Texture.h"
#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform {
	
	void Win_Texture::LoadFromFile(const std::filesystem::path& m_path, const bool flip) {
		// Load Texture
		stbi_set_flip_vertically_on_load(flip);

		unsigned char* bytes = stbi_load(m_path.string().c_str(), &imgWidth, &imgHeight, &numColCh, 0);

		LX_CORE_TRACE("{},{},{}", imgWidth, imgHeight, numColCh);
		if (!bytes) { LX_CORE_ERROR("BYTES NULLPTR"); return; }

		glGenTextures(1, &texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		GLenum colorMode = GL_RGB;
		switch (numColCh)
		{
		case 1:
			colorMode = GL_RED;
			break;
		case 2:
			colorMode = GL_RG;
			break;
		case 3:
			colorMode = GL_RGB;
			break;
		case 4:
			colorMode = GL_RGBA;
			break;
		}

		LX_CORE_TRACE("Creating tex");
		if (bytes) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, colorMode, imgWidth, imgHeight, 0, colorMode, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			LX_CORE_ERROR("Failed to load image: {}", m_path.string());
			return;
		}
		LX_CORE_TRACE("Done");
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D, 0);

		valid = true;

		// Reset flip vertically on load since the next might not want to
		stbi_set_flip_vertically_on_load(false);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void Win_Texture::Unload() {
		// Unload Texture
		valid = false;
	}

	void Win_Texture::Use() {
		if (!IsValid()) { LX_CORE_ERROR("Tried to use unvalid texture: {}", path); }
		glBindTexture(GL_TEXTURE_2D, texID);
		// Bind currently used texture to be this
	}

	void Win_Texture::Delete() {
		// Delete Texture
		glDeleteTextures(1, &texID);
	}
}