#pragma once


// Definitions for different platforms
#ifdef LUXIA_PLATFORM_WINDOWS
	#ifdef LUXIA_EXPORT // export or import dll
	#define LUXIA_API __declspec(dllexport) 
	#define GLM_ENABLE_EXPERIMENTAL
	#else
	#define LUXIA_API __declspec(dllimport)
	#endif
#else
#error Luxia doesnt support your OS!
#endif // LUXIA_PLATFORM_WINDOWS