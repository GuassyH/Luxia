#pragma once

#include "Core.h"
#include "Application.h"
#include "Log.h"

#ifdef LUXIA_PLATFORM_WINDOWS

extern std::unique_ptr<Luxia::Application> Luxia::CreateApplication();

int main(int argc, char** argv)
{
	Luxia::Log::Init();
	LX_CORE_WARN("Initialized Logger");

	// Create the application using externally defined function
	std::unique_ptr<Luxia::Application> app = Luxia::CreateApplication();

	app->CoreStartup();
	app->Startup();
	app->Run();
	app->Shutdown();
	app->CoreShutdown();

	return 0;
}

#endif // LUXIA_PLATFORM_WINDOWS