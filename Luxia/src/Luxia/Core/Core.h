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


#if defined(_MSC_VER)
#define LX_DEBUG_BREAK() __debugbreak()
#else
#include <signal.h>
#define LX_DEBUG_BREAK() raise(SIGTRAP)
#endif


// Enable asserts in debug builds (adjust if you want asserts in release)
#if defined(LUXIA_DEBUG)

	// Simple assert: logs the expression text
#define LX_CORE_ASSERT(cond) \
        do { \
            if (!(cond)) { \
                LX_CORE_ERROR("Assertion Failed: {}", #cond); \
                LX_DEBUG_BREAK(); \
                std::abort(); \
            } \
        } while (0)

	// Assert with custom message (use same format style as LX_CORE_ERROR)
	// Example: LX_CORE_ASSERT_MSG(ptr != nullptr, "ptr was null for id {}", id);
#define LX_CORE_ASSERT_MSG(cond, ...) \
        do { \
            if (!(cond)) { \
                LX_CORE_ERROR(__VA_ARGS__); \
                LX_DEBUG_BREAK(); \
                std::abort(); \
            } \
        } while (0)

#else

	// No-op in non-debug builds (change to still abort if you prefer)
#define LX_CORE_ASSERT(cond) (void)0
#define LX_CORE_ASSERT_MSG(cond, ...) (void)0

#endif


/// Input Definitions

#ifdef LUXIA_RENDERER_OPENGL
	#define LX_KEY_A GLFW_KEY_A 
	#define LX_KEY_B GLFW_KEY_B 
	#define LX_KEY_C GLFW_KEY_C 
	#define LX_KEY_D GLFW_KEY_D 
	#define LX_KEY_E GLFW_KEY_E 
	#define LX_KEY_F GLFW_KEY_F 
	#define LX_KEY_G GLFW_KEY_G 
	#define LX_KEY_H GLFW_KEY_H 
	#define LX_KEY_I GLFW_KEY_I 
	#define LX_KEY_J GLFW_KEY_J 
	#define LX_KEY_K GLFW_KEY_K 
	#define LX_KEY_L GLFW_KEY_L 
	#define LX_KEY_M GLFW_KEY_M 
	#define LX_KEY_N GLFW_KEY_N 
	#define LX_KEY_O GLFW_KEY_O 
	#define LX_KEY_P GLFW_KEY_P 
	#define LX_KEY_Q GLFW_KEY_Q 
	#define LX_KEY_R GLFW_KEY_R 
	#define LX_KEY_S GLFW_KEY_S 
	#define LX_KEY_T GLFW_KEY_T 
	#define LX_KEY_U GLFW_KEY_U 
	#define LX_KEY_V GLFW_KEY_V 
	#define LX_KEY_W GLFW_KEY_W 
	#define LX_KEY_X GLFW_KEY_X 
	#define LX_KEY_Y GLFW_KEY_Y 
	#define LX_KEY_Z GLFW_KEY_Z 
	#define LX_KEY_0 GLFW_KEY_0 
	#define LX_KEY_1 GLFW_KEY_1 
	#define LX_KEY_2 GLFW_KEY_2 
	#define LX_KEY_3 GLFW_KEY_3 
	#define LX_KEY_4 GLFW_KEY_4 
	#define LX_KEY_5 GLFW_KEY_5 
	#define LX_KEY_6 GLFW_KEY_6 
	#define LX_KEY_7 GLFW_KEY_7 
	#define LX_KEY_8 GLFW_KEY_8 
	#define LX_KEY_9 GLFW_KEY_9 
	#define LX_KEY_ESCAPE			GLFW_KEY_ESCAPE			
	#define LX_KEY_ENTER			GLFW_KEY_ENTER			
	#define LX_KEY_TAB				GLFW_KEY_TAB				
	#define LX_KEY_BACKSPACE		GLFW_KEY_BACKSPACE		
	#define LX_KEY_INSERT			GLFW_KEY_INSERT			
	#define LX_KEY_DELETE			GLFW_KEY_DELETE			
	#define LX_KEY_RIGHT			GLFW_KEY_RIGHT			
	#define LX_KEY_LEFT				GLFW_KEY_LEFT				
	#define LX_KEY_DOWN				GLFW_KEY_DOWN				
	#define LX_KEY_UP				GLFW_KEY_UP				
	#define LX_KEY_PAGE_UP			GLFW_KEY_PAGE_UP			
	#define LX_KEY_PAGE_DOWN		GLFW_KEY_PAGE_DOWN		
	#define LX_KEY_HOME				GLFW_KEY_HOME				
	#define LX_KEY_END				GLFW_KEY_END				
	#define LX_KEY_CAPS_LOCK		GLFW_KEY_CAPS_LOCK		
	#define LX_KEY_SCROLL_LOCK		GLFW_KEY_SCROLL_LOCK		
	#define LX_KEY_NUM_LOCK			GLFW_KEY_NUM_LOCK			
	#define LX_KEY_PRINT_SCREEN		GLFW_KEY_PRINT_SCREEN		
	#define LX_KEY_PAUSE			GLFW_KEY_PAUSE			
	#define LX_KEY_F1				GLFW_KEY_F1				
	#define LX_KEY_F2				GLFW_KEY_F2				
	#define LX_KEY_F3				GLFW_KEY_F3				
	#define LX_KEY_F4				GLFW_KEY_F4				
	#define LX_KEY_F5				GLFW_KEY_F5				
	#define LX_KEY_F6				GLFW_KEY_F6				
	#define LX_KEY_F7				GLFW_KEY_F7				
	#define LX_KEY_F8				GLFW_KEY_F8				
	#define LX_KEY_F9				GLFW_KEY_F9				
	#define LX_KEY_F10				GLFW_KEY_F10				
	#define LX_KEY_F11				GLFW_KEY_F11				
	#define LX_KEY_F12				GLFW_KEY_F12				
	#define LX_KEY_F13				GLFW_KEY_F13				
	#define LX_KEY_F14				GLFW_KEY_F14				
	#define LX_KEY_F15				GLFW_KEY_F15				
	#define LX_KEY_F16				GLFW_KEY_F16				
	#define LX_KEY_F17				GLFW_KEY_F17				
	#define LX_KEY_F18				GLFW_KEY_F18				
	#define LX_KEY_F19				GLFW_KEY_F19				
	#define LX_KEY_F20				GLFW_KEY_F20				
	#define LX_KEY_F21				GLFW_KEY_F21				
	#define LX_KEY_F22				GLFW_KEY_F22				
	#define LX_KEY_F23				GLFW_KEY_F23				
	#define LX_KEY_F24				GLFW_KEY_F24				
	#define LX_KEY_F25				GLFW_KEY_F25				
	#define LX_KEY_KP_0				GLFW_KEY_KP_0				/* Keypad */
	#define LX_KEY_KP_1				GLFW_KEY_KP_1				/* Keypad */
	#define LX_KEY_KP_2				GLFW_KEY_KP_2				/* Keypad */
	#define LX_KEY_KP_3				GLFW_KEY_KP_3				/* Keypad */
	#define LX_KEY_KP_4				GLFW_KEY_KP_4				/* Keypad */
	#define LX_KEY_KP_5				GLFW_KEY_KP_5				/* Keypad */
	#define LX_KEY_KP_6				GLFW_KEY_KP_6				/* Keypad */
	#define LX_KEY_KP_7				GLFW_KEY_KP_7				/* Keypad */
	#define LX_KEY_KP_8				GLFW_KEY_KP_8				/* Keypad */
	#define LX_KEY_KP_9				GLFW_KEY_KP_9				/* Keypad */
	#define LX_KEY_KP_DECIMAL		GLFW_KEY_KP_DECIMAL		/* Keypad */
	#define LX_KEY_KP_DIVIDE		GLFW_KEY_KP_DIVIDE		/* Keypad */
	#define LX_KEY_KP_MULTIPLY		GLFW_KEY_KP_MULTIPLY		/* Keypad */
	#define LX_KEY_KP_SUBTRACT		GLFW_KEY_KP_SUBTRACT		/* Keypad */
	#define LX_KEY_KP_ADD			GLFW_KEY_KP_ADD			/* Keypad */
	#define LX_KEY_KP_ENTER			GLFW_KEY_KP_ENTER			/* Keypad */
	#define LX_KEY_KP_EQUAL			GLFW_KEY_KP_EQUAL			/* Keypad */
	#define LX_KEY_LEFT_SHIFT		GLFW_KEY_LEFT_SHIFT		
	#define LX_KEY_LEFT_CONTROL		GLFW_KEY_LEFT_CONTROL		
	#define LX_KEY_LEFT_ALT			GLFW_KEY_LEFT_ALT			
	#define LX_KEY_LEFT_SUPER		GLFW_KEY_LEFT_SUPER		
	#define LX_KEY_RIGHT_SHIFT		GLFW_KEY_RIGHT_SHIFT		
	#define LX_KEY_RIGHT_CONTROL	GLFW_KEY_RIGHT_CONTROL	
	#define LX_KEY_RIGHT_ALT		GLFW_KEY_RIGHT_ALT		
	#define LX_KEY_RIGHT_SUPER		GLFW_KEY_RIGHT_SUPER		
	#define LX_KEY_MENU				GLFW_KEY_MENU				
	#define LX_MOUSE_BUTTON_1 GLFW_MOUSE_BUTTON_1  /* left */
	#define LX_MOUSE_BUTTON_2 GLFW_MOUSE_BUTTON_2  /* right */
	#define LX_MOUSE_BUTTON_3 GLFW_MOUSE_BUTTON_3  /* middle */
	#define LX_MOUSE_BUTTON_4 GLFW_MOUSE_BUTTON_4  /* back */
#else
	/* Printable Keys */
	#define LX_KEY_A constexpr 0
	#define LX_KEY_B constexpr 1
	#define LX_KEY_C constexpr 2
	#define LX_KEY_D constexpr 3
	#define LX_KEY_E constexpr 4
	#define LX_KEY_F constexpr 5
	#define LX_KEY_G constexpr 6
	#define LX_KEY_H constexpr 7
	#define LX_KEY_I constexpr 8
	#define LX_KEY_J constexpr 9
	#define LX_KEY_K constexpr 10
	#define LX_KEY_L constexpr 11
	#define LX_KEY_M constexpr 12
	#define LX_KEY_N constexpr 13
	#define LX_KEY_O constexpr 14
	#define LX_KEY_P constexpr 15
	#define LX_KEY_Q constexpr 16
	#define LX_KEY_R constexpr 17
	#define LX_KEY_S constexpr 18
	#define LX_KEY_T constexpr 19
	#define LX_KEY_U constexpr 20
	#define LX_KEY_V constexpr 21
	#define LX_KEY_W constexpr 22
	#define LX_KEY_X constexpr 23
	#define LX_KEY_Y constexpr 24
	#define LX_KEY_Z constexpr 25

	#define LX_KEY_0 constexpr 26
	#define LX_KEY_1 constexpr 27
	#define LX_KEY_2 constexpr 28
	#define LX_KEY_3 constexpr 29
	#define LX_KEY_4 constexpr 30
	#define LX_KEY_5 constexpr 31
	#define LX_KEY_6 constexpr 32
	#define LX_KEY_7 constexpr 33
	#define LX_KEY_8 constexpr 34
	#define LX_KEY_9 constexpr 35

	/* Functional Keys */
	#define LX_KEY_ESCAPE			constexpr 36
	#define LX_KEY_ENTER			constexpr 37
	#define LX_KEY_TAB				constexpr 38
	#define LX_KEY_BACKSPACE		constexpr 39
	#define LX_KEY_INSERT			constexpr 40
	#define LX_KEY_DELETE			constexpr 41
	#define LX_KEY_RIGHT			constexpr 42
	#define LX_KEY_LEFT				constexpr 43
	#define LX_KEY_DOWN				constexpr 44
	#define LX_KEY_UP				constexpr 45
	#define LX_KEY_PAGE_UP			constexpr 46
	#define LX_KEY_PAGE_DOWN		constexpr 47
	#define LX_KEY_HOME				constexpr 48
	#define LX_KEY_END				constexpr 49
	#define LX_KEY_CAPS_LOCK		constexpr 50
	#define LX_KEY_SCROLL_LOCK		constexpr 51
	#define LX_KEY_NUM_LOCK			constexpr 52
	#define LX_KEY_PRINT_SCREEN		constexpr 53
	#define LX_KEY_PAUSE			constexpr 54
	#define LX_KEY_F1				constexpr 55
	#define LX_KEY_F2				constexpr 56
	#define LX_KEY_F3				constexpr 57
	#define LX_KEY_F4				constexpr 58
	#define LX_KEY_F5				constexpr 59
	#define LX_KEY_F6				constexpr 60
	#define LX_KEY_F7				constexpr 61
	#define LX_KEY_F8				constexpr 62
	#define LX_KEY_F9				constexpr 63
	#define LX_KEY_F10				constexpr 64
	#define LX_KEY_F11				constexpr 65
	#define LX_KEY_F12				constexpr 66
	#define LX_KEY_F13				constexpr 67
	#define LX_KEY_F14				constexpr 68
	#define LX_KEY_F15				constexpr 69
	#define LX_KEY_F16				constexpr 70
	#define LX_KEY_F17				constexpr 71
	#define LX_KEY_F18				constexpr 72
	#define LX_KEY_F19				constexpr 73
	#define LX_KEY_F20				constexpr 74
	#define LX_KEY_F21				constexpr 75
	#define LX_KEY_F22				constexpr 76
	#define LX_KEY_F23				constexpr 77
	#define LX_KEY_F24				constexpr 78
	#define LX_KEY_F25				constexpr 79
	#define LX_KEY_KP_0				constexpr 80 /* Keypad */
	#define LX_KEY_KP_1				constexpr 81 /* Keypad */
	#define LX_KEY_KP_2				constexpr 82 /* Keypad */
	#define LX_KEY_KP_3				constexpr 83 /* Keypad */
	#define LX_KEY_KP_4				constexpr 84 /* Keypad */
	#define LX_KEY_KP_5				constexpr 85 /* Keypad */
	#define LX_KEY_KP_6				constexpr 86 /* Keypad */
	#define LX_KEY_KP_7				constexpr 87 /* Keypad */
	#define LX_KEY_KP_8				constexpr 88 /* Keypad */
	#define LX_KEY_KP_9				constexpr 89 /* Keypad */
	#define LX_KEY_KP_DECIMAL		constexpr 90 /* Keypad */
	#define LX_KEY_KP_DIVIDE		constexpr 91 /* Keypad */
	#define LX_KEY_KP_MULTIPLY		constexpr 92 /* Keypad */
	#define LX_KEY_KP_SUBTRACT		constexpr 93 /* Keypad */
	#define LX_KEY_KP_ADD			constexpr 94 /* Keypad */
	#define LX_KEY_KP_ENTER			constexpr 95 /* Keypad */
	#define LX_KEY_KP_EQUAL			constexpr 96 /* Keypad */
	#define LX_KEY_LEFT_SHIFT		constexpr 97
	#define LX_KEY_LEFT_CONTROL		constexpr 98
	#define LX_KEY_LEFT_ALT			constexpr 99
	#define LX_KEY_LEFT_SUPER		constexpr 100
	#define LX_KEY_RIGHT_SHIFT		constexpr 101
	#define LX_KEY_RIGHT_CONTROL	constexpr 102
	#define LX_KEY_RIGHT_ALT		constexpr 103
	#define LX_KEY_RIGHT_SUPER		constexpr 104
	#define LX_KEY_MENU				constexpr 105


	/* Mouse */
	// (1 = left, 2 = right, 3 = middle, 4 = back, 5 = forward)
	#define LX_MOUSE_BUTTON_1 constexpr 0 /* left */
	#define LX_MOUSE_BUTTON_2 constexpr 1 /* right */
	#define LX_MOUSE_BUTTON_3 constexpr 2 /* middle */
	#define LX_MOUSE_BUTTON_4 constexpr 3 /* back */
	#define LX_MOUSE_BUTTON_5 constexpr 4 /* left */

#endif
