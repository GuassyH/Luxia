#include "Win_Window.h"

#include <stdio.h>


namespace Luxia::Platform {

	Win_Window::Win_Window() = default;
	Win_Window::~Win_Window() = default;

	void Win_Window::Create(int width, int height, const std::string& title)
	{
		m_Width = width;
		m_Height = height;
		m_Title = title;

		printf("Created Windows Window: %s (%dx%d)\n", m_Title.c_str(), m_Width, m_Height);
	}
	void Win_Window::BeginFrame()
	{
		// Windows specific begin frame code
	}
	void Win_Window::EndFrame()
	{
		// Windows specific end frame code
	}
	bool Win_Window::ShouldClose() const
	{
		// For demonstration, we'll just return false to keep the window open
		return false;
	}
}
