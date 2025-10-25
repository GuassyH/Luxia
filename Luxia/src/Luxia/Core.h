#pragma once


// Definitions for different platforms
#ifdef LUXIA_PLATFORM_WINDOWS
	#ifdef LUXIA_EXPORT 
	#define LUXIA_API __declspec(dllexport) // export or import dll
	#else
	#define LUXIA_API __declspec(dllimport)
	#endif
#else
#error Luxia doesnt support your OS!
#endif // LUXIA_PLATFORM_WINDOWS