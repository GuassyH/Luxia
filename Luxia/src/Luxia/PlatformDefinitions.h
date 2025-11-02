#pragma once

#include "Core.h"
#include "Platform/IWindow.h"
#include "Platform/Win_Window.h"

#include "Platform/ITexture.h"
#include "Platform/Win_Texture.h"

#include "Platform/IModel.h"
#include "Platform/Win_Model.h"

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

	inline std::shared_ptr<Luxia::IModel> CreateModel() {
		std::shared_ptr<Luxia::IModel> mod;

		#ifdef LUXIA_PLATFORM_WINDOWS
			mod = std::make_shared<Luxia::Platform::Win_Model>();
		#else 
		#error Luxia doesnt support your OS!
		#endif

		return mod;
	}

}

#include "Luxia/Rendering/RenderingBuffers.h"
#include "Luxia/Rendering/OpenGLRendering.h"

namespace Luxia::Rendering {

	inline std::shared_ptr<Luxia::Rendering::VAO> CreateVAO() {
		std::shared_ptr<Luxia::Rendering::VAO> vao;
		#ifdef LUXIA_PLATFORM_WINDOWS
			vao = std::make_shared<Luxia::Rendering::OpenGL::OpenGL_VAO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return vao;
	}

	inline std::shared_ptr<Luxia::Rendering::VBO> CreateVBO() {
		std::shared_ptr<Luxia::Rendering::VBO> vbo;
		#ifdef LUXIA_PLATFORM_WINDOWS
				vbo = std::make_shared<Luxia::Rendering::OpenGL::OpenGL_VBO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return vbo;
	}

	inline std::shared_ptr<Luxia::Rendering::EBO> CreateEBO() {
		std::shared_ptr<Luxia::Rendering::EBO> ebo;
		#ifdef LUXIA_PLATFORM_WINDOWS
			ebo = std::make_shared<Luxia::Rendering::OpenGL::OpenGL_EBO>();
		#endif // LUXIA_PLATFORM_WINDOWS

		return ebo;
	}
}