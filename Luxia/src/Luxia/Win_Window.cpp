#include "Win_Window.h"

#include "Log.h"
#include <stdio.h>

namespace Luxia::Platform {

	Win_Window::Win_Window() = default;
	Win_Window::~Win_Window() = default;

	int Win_Window::Create(int width, int height, const std::string& title)
	{
		m_Width = width;
		m_Height = height;
		m_Title = title;

		if (!glfwInit()) { return -1; }

		// Set opengl version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		m_Monitor = glfwGetWindowMonitor(m_Window);

		if (!m_Window) {
			LX_CORE_ERROR("Failed to create Window");
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(0); // Turn off v-sync

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LX_CORE_ERROR("Failed to initialize GLAD");
			return -1;
		}

		LX_CORE_TRACE("OpenGL Info: ");
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl << "\n";

		// Get monitor height
		 const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		 m_MonitorWidth = mode->width;
		 m_MonitorHeight = mode->height;

		// Set Frame Buffer Size Callback

		// Culling stuff
		glViewport(0, 0, m_Width, m_Height);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);


		LX_CORE_INFO("Created Windows Window: {} ({}x{})", m_Title.c_str(), m_Width, m_Height);

		return 1;
	}
	
	void Win_Window::BeginFrame()
	{
		glClearColor(0.3f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_Width, m_Height);
	}

	void Win_Window::EndFrame()
	{
		if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		// Windows specific end frame code
	}

	void Win_Window::SetTitle(const std::string& title)
	{
		m_Title = title;
		glfwSetWindowTitle(m_Window, m_Title.c_str());
	}

	bool Win_Window::ShouldClose() const {
		return glfwWindowShouldClose(m_Window);
	}
}
