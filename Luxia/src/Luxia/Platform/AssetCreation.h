#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Rendering/API/IWindow.h"
#include "Luxia/Rendering/API/ITexture.h"
#include "Luxia/Rendering/API/IModel.h"
#include "Luxia/Rendering/API/ICamera.h"
#include "Luxia/Rendering/API/IShader.h"
#include "Luxia/Rendering/API/IMaterial.h"
#include "Luxia/Events/EventHandler.h"


#ifdef LUXIA_RENDERER_OPENGL
	#include "Luxia/Platform/OpenGL/GL_Window.h"
	#include "Luxia/Platform/OpenGL/GL_Texture.h"
	#include "Luxia/Platform/OpenGL/GL_Model.h"
	#include "Luxia/Platform/OpenGL/GL_Camera.h"
	#include "Luxia/Platform/OpenGL/GL_Shader.h"
	#include "Luxia/Platform/OpenGL/GL_Material.h"

namespace Luxia::Platform::Assets {
	inline std::shared_ptr<ICamera> LUXIA_API CreateCamera(auto&&... args) {
		return std::make_shared<Platform::OpenGL::GL_Camera>(std::forward<decltype(args)>(args)...);
	}

	inline std::shared_ptr<IWindow> LUXIA_API CreateAppWindow(int width, int height, std::string name) {
		auto win = std::make_shared<Platform::OpenGL::GL_Window>();
		win->Create(width, height, name);
		return win;
	}

	inline std::shared_ptr<ITexture> LUXIA_API CreateTexture() {
		return std::make_shared<Platform::OpenGL::GL_Texture>();
	}

	inline std::shared_ptr<IModel> LUXIA_API CreateModel() {
		return std::make_shared<Platform::OpenGL::GL_Model>();
	}

	inline std::shared_ptr<IMaterial> LUXIA_API CreateMaterial(auto&&... args) {
		return std::make_shared<Platform::OpenGL::GL_Material>(std::forward<decltype(args)>(args)...);
	}

	inline std::shared_ptr<IShader> LUXIA_API CreateShader(auto&&... args) {
		return std::make_shared<Platform::OpenGL::GL_Shader>(std::forward<decltype(args)>(args)...);
	}
}

#elif LUXIA_RENDERER_VULKAN
	#error Vulkan Renderer not yet supported in Luxia!
#endif

