#pragma once
#include "IWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KHR/khrplatform.h>

namespace Luxia::Platform {
	class Win_Window : public Luxia::IWindow {
	public:
		Win_Window();
		~Win_Window();

		int m_Width;
		int m_Height;
		std::string m_Title;

		GLFWwindow* m_Window = nullptr;
		GLFWmonitor* m_Monitor = nullptr;

		// void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

		virtual int Create(int width, int height, const std::string& title) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual bool ShouldClose() const override;
	};
}

