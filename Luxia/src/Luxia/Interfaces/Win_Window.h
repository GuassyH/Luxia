#pragma once
#include "IWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <KHR/khrplatform.h>
#include <Windows.h> 

namespace Luxia::Platform {
	class Win_Window : public Luxia::IWindow {
	public:
		Win_Window();
		~Win_Window();

		std::string m_Title;

		GLFWwindow* m_Window = nullptr;
		GLFWmonitor* m_Monitor = nullptr;

		virtual int Create(int width, int height, const std::string& title) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Close() override;

		virtual void SetTitle(const std::string& title) override;

		virtual void OnEvent(Event& e) override;
	protected:
		virtual bool ResizeEvent(WindowResizeEvent& e) override;
	};
}

