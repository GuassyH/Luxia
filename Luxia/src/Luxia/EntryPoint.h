#pragma once
#include <stdio.h>

#include "Core.h"
#include "Application.h"

#ifdef LUXIA_PLATFORM_WINDOWS

extern Luxia::Application* Luxia::CreateApplication();

int main(int argc, char** argv)
{
	printf("Started App!\n");

	Luxia::Application* app = Luxia::CreateApplication();
	app->Run();
	delete app;
	
	printf("Ended App!\n");
	return 0;
}

#endif // LUXIA_PLATFORM_WINDOWS