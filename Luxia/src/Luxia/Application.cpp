#include "Application.h"

namespace Luxia
{
	Application::Application()
	{
	#ifdef LUXIA_PLATFORM_WINDOWS
		m_Window = std::make_unique<Win_Window>();
	#endif
	}
	Application::~Application()
	{
		m_Window->~IWindow();
	}
	void Application::Run()
	{
		while (true) {
			m_Window->Render();
		}
	}
}
