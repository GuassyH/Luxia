#pragma once

#include "Core.h"
#include "IWindow.h"
#include "Win_Window.h"

#include <iostream>

// This namespace is for platform-specific implementations
// So i can call Luxia::Platform::CreateWindow() to get the correct window for the platform
// IM NOT GOING TO FOCUS ON THIS NOW... ITS JUST A TEST
namespace Luxia::Platform
{
	std::shared_ptr<Luxia::IWindow> CreateWindow()
	{
		#ifdef LUXIA_PLATFORM_WINDOWS
		return std::make_shared<Luxia::Platform::Win_Window>();
		#else
		#error Luxia doesnt support your OS!
		#endif
	}
}