#pragma once

#include "Luxia/Core.h"
#include "Luxia/Rendering/API/IWindow.h"
#include "Luxia/Platform/OpenGL/GL_Window.h"

#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Platform/OpenGL/GL_Texture.h"

#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Platform/OpenGL/GL_Model.h"

#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Platform/OpenGL/GL_Camera.h"

#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Platform/OpenGL/GL_Shader.h"

#include <iostream>

namespace Luxia::Platform::Assets
{
	// Create the window, with width, height, and title, this will change automatically based on the OS
	inline std::shared_ptr<Luxia::IWindow> CreateAppWindow(int width, int height, std::string title)
	{
		std::shared_ptr<Luxia::IWindow> win;

		#ifdef LUXIA_RENDERER_OPENGL
			win = std::make_shared<Luxia::Platform::OpenGL::GL_Window>();
		#else 
		#error Luxia doesnt support your Renderer!
		#endif

		win->Create(1920, 1080, "Luxia Application");
		return win;
	}

	inline std::shared_ptr<Luxia::ITexture> CreateTexture() {
		std::shared_ptr<Luxia::ITexture> tex;

		#ifdef LUXIA_RENDERER_OPENGL
			tex = std::make_shared<Luxia::Platform::OpenGL::GL_Texture>();
		#else 
		#error Luxia doesnt support your Renderer!
		#endif

		return tex;
	}

	inline std::shared_ptr<Luxia::IModel> CreateModel() {
		std::shared_ptr<Luxia::IModel> mod;

		#ifdef LUXIA_RENDERER_OPENGL
			mod = std::make_shared<Luxia::Platform::OpenGL::GL_Model>();
		#else 
		#error Luxia doesnt support your Renderer!
		#endif

		return mod;
	}

	inline std::shared_ptr<Luxia::ICamera> CreateCamera(int width, int height) {
		std::shared_ptr<Luxia::ICamera> cam;

		#ifdef LUXIA_RENDERER_OPENGL
			cam = std::make_shared<Luxia::Platform::OpenGL::GL_Camera>(width, height);
		#else 
		#error Luxia doesnt support your Renderer!
		#endif

		return cam;
	}

	inline std::shared_ptr<Luxia::IShader> CreateShader(const char* fragSrc, const char* vertSrc) {
		std::shared_ptr<Luxia::IShader> shader;

		#ifdef LUXIA_RENDERER_OPENGL
			shader = std::make_shared<Luxia::Platform::OpenGL::GL_Shader>(fragSrc, vertSrc);
		#else 
		#error Luxia doesnt support your Renderer!
		#endif

		return shader;
	}
}