#include "lxpch.h"
#include "GL_Texture.h"

#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Luxia::Platform::OpenGL {
	
	
	void GL_Texture::CreateDepthTex(const int width, const int height) {
		// Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Create Texture and bind
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); // Should use SRGB?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// enable depth comparison (for sampler2DShadow)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texID, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FBO incomplete: " << std::hex << status << std::dec << std::endl;
			valid = false;
			return;
		}

		// All of the FBO data and such is sent back to the input params
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		imgWidth = width;
		imgHeight = height;
		numColCh = 1;

		valid = true;
		hasPath = false;
		is_fbo_tex = true;
	}


	void GL_Texture::CreateFBOTex(const int width, const int height) {
		// Create FBO
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Create Texture and bind
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Should use SRGB?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		// enable depth comparison (for sampler2DShadow)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

		// Create RBO for depth and stencil we need depth for 3D rendering)
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FBO incomplete: " << std::hex << status << std::dec << std::endl;
			valid = false;
			return;
		}


		// All of the FBO data and such is sent back to the input params
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		imgWidth = width;
		imgHeight = height;
		numColCh = 4;

		valid = true;
		hasPath = false;
		is_fbo_tex = true;
	}


	void GL_Texture::CreateEmpty(const int width, const int height, const int colCh) {
		imgWidth = width;
		imgHeight = height;
		numColCh = colCh;

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

		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, imgWidth, imgHeight, 0, colorMode, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		valid = true;
		hasPath = false;
	}

	void GL_Texture::LoadFromFile(const std::filesystem::path& m_path, const bool m_flip) {
		// Load Texture
		flip = m_flip;

		stbi_set_flip_vertically_on_load(flip);

		unsigned char* bytes = stbi_load(m_path.lexically_normal().string().c_str(), &imgWidth, &imgHeight, &numColCh, 0);

		if (!bytes) { LX_CORE_ERROR("Texture Loading failed (bytes null): {}", m_path.string()); return; }

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
			glTexImage2D(GL_TEXTURE_2D, 0, colorMode, imgWidth, imgHeight, 0, colorMode, GL_UNSIGNED_BYTE, bytes);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Which filter type?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else {
			LX_CORE_ERROR("Failed to load image: {}", m_path.string());
			return;
		}
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D, 0);

		valid = true;
		hasPath = true;
		path = m_path;

		// Reset flip vertically on load since the next might not want to
		stbi_set_flip_vertically_on_load(false);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	void GL_Texture::Delete() {
		// Unload Texture
		hasPath = false;
		valid = false;
		glDeleteTextures(1, &texID);

		if (IsFBOTex()) {
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &rbo);
		}

		is_fbo_tex = false;
	}


	void GL_Texture::Bind() {
		if (!IsValid()) { LX_CORE_ERROR("Tried to use unvalid texture: {}", path.string()); }
		glBindTexture(GL_TEXTURE_2D, texID);
		// Bind currently used texture to be this
	}

	void GL_Texture::Unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}