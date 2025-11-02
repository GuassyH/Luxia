#pragma once

#include "Core.h"
#include "Platform/IWindow.h"
#include "Platform/Win_Window.h"

#include "Platform/ITexture.h"
#include "Platform/Win_Texture.h"

#include <iostream>

// This namespace is for platform-specific implementations
// So i can call Luxia::Platform::CreateWindow() to get the correct window for the platform
// IM NOT GOING TO FOCUS ON THIS NOW... ITS JUST A TEST
namespace Luxia::Platform
{
	// Create the window, with width, height, and title, this will change automatically based on the OS
	inline std::shared_ptr<Luxia::IWindow> CreateAppWindow(int width, int height, std::string title)
	{
		std::shared_ptr<Luxia::IWindow> win;
		#ifdef LUXIA_PLATFORM_WINDOWS
			win = std::make_shared<Luxia::Platform::Win_Window>();
		#else 
		#error Luxia doesnt support your OS!
		#endif
		
		win->Create(1920, 1080, "Luxia Application");
		return win;
	}

	inline std::shared_ptr<Luxia::ITexture> CreateTexture() {
		std::shared_ptr<Luxia::ITexture> tex;

		#ifdef LUXIA_PLATFORM_WINDOWS
			tex = std::make_shared<Luxia::Platform::Win_Texture>();
		#else 
		#error Luxia doesnt support your OS!
		#endif

		return tex;
	}

}