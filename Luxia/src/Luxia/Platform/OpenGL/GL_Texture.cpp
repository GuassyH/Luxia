#include "GL_Texture.h"
#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform::OpenGL {
	
	void GL_Texture::LoadFromFile(const std::filesystem::path& m_path, const bool flip) {
		// Load Texture
		stbi_set_flip_vertically_on_load(flip);

		unsigned char* bytes = stbi_load(m_path.string().c_str(), &imgWidth, &imgHeight, &numColCh, 0);

		if (!bytes) { LX_CORE_ERROR("BYTES NULLPTR"); return; }

		glGenTextures(1, &texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Creation crashes if pixel allignment needs to be divisible by 4

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
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D, 0);

		valid = true;

		// Reset flip vertically on load since the next might not want to
		stbi_set_flip_vertically_on_load(false);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GL_Texture::Unload() {
		// Unload Texture
		valid = false;
	}

	void GL_Texture::Use() {
		if (!IsValid()) { LX_CORE_ERROR("Tried to use unvalid texture: {}", path.string()); }
		glBindTexture(GL_TEXTURE_2D, texID);
		// Bind currently used texture to be this
	}

	void GL_Texture::Delete() {
		// Delete Texture
		glDeleteTextures(1, &texID);
	}
}