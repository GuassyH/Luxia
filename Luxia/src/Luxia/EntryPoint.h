#pragma once
#include <stdio.h>

#include "Core.h"
#include "Application.h"

#ifdef LUXIA_PLATFORM_WINDOWS

extern Luxia::Application* Luxia::CreateApplication();

int main(int argc, char** argv)
{
	printf("Started Luxia App!\n\n");

	Luxia::Application* app = Luxia::CreateApplication();
	app->Run();
	delete app;
	
	printf("\nEnded Luxia App, see you next time!\n");
	return 0;
}

#endif // LUXIA_PLATFORM_WINDOWS