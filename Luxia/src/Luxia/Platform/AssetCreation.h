#pragma once

#include "Luxia/Core.h"
#include "Luxia/Rendering/API/IWindow.h"
#include "Luxia/Platform/OpenGL/GL_Window.h"

#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Platform/OpenGL/GL_Texture.h"

#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Platform/OpenGL/GL_Model.h"

#include <iostream>

namespace Luxia::Platform::Assets
{
	// Create the window, with width, height, and title, this will change automatically based on the OS
	inline std::shared_ptr<Luxia::IWindow> CreateAppWindow(int width, int height, std::string title)
	{
		std::shared_ptr<Luxia::IWindow> win;

		#ifdef LUXIA_PLATFORM_WINDOWS
				win = std::make_shared<Luxia::Platform::GL_Window>();
		#else 
		#error Luxia doesnt support your OS!
		#endif

		win->Create(1920, 1080, "Luxia Application");
		return win;
	}

	inline std::shared_ptr<Luxia::ITexture> CreateTexture() {
		std::shared_ptr<Luxia::ITexture> tex;

		#ifdef LUXIA_PLATFORM_WINDOWS
				tex = std::make_shared<Luxia::Platform::GL_Texture>();
		#else 
		#error Luxia doesnt support your OS!
		#endif

		return tex;
	}

	inline std::shared_ptr<Luxia::IModel> CreateModel() {
		std::shared_ptr<Luxia::IModel> mod;

		#ifdef LUXIA_PLATFORM_WINDOWS
				mod = std::make_shared<Luxia::Platform::GL_Model>();
		#else 
		#error Luxia doesnt support your OS!
		#endif

		return mod;
	}

}