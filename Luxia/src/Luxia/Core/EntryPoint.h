#pragma once

#include "Core.h"
#include "Application.h"
#include "Log.h"

#ifdef LUXIA_PLATFORM_WINDOWS

extern Luxia::Application* Luxia::CreateApplication();

int main(int argc, char** argv)
{
	Luxia::Log::Init();
	LX_CORE_WARN("Initialized Logger");

	// Create the application using externally defined function
	Luxia::Application* app = Luxia::CreateApplication();
	app->Run();
	delete app;
	
	return 0;
}

#endif // LUXIA_PLATFORM_WINDOWS