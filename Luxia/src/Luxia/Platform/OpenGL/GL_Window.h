#pragma once
#include "Luxia/Rendering/API/IWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KHR/khrplatform.h>
#include <Windows.h> 

namespace Luxia::Platform::OpenGL {
	class GL_Window : public Luxia::IWindow {
	public:
		GL_Window();
		~GL_Window();

		std::string m_Title;

		GLFWwindow* m_Window = nullptr;
		GLFWmonitor* m_Monitor = nullptr;

		virtual int Create(const int width, const int height, const std::string& title) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Close() override;

		virtual void SetTitle(const std::string& title) override;

		virtual void OnEvent(Event& e) override;
	protected:
		virtual bool ResizeEvent(WindowResizeEvent& e) override;
	};
}

