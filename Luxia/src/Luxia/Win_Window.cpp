#include "Win_Window.h"

#include <stdio.h>

namespace Luxia::Platform {

	Win_Window::Win_Window() = default;
	Win_Window::~Win_Window() = default;

	void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		std::cout << "window size is " << width << " x " << height << std::endl;
	}

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
			std::cout << "Failed to create Window" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(0); // Turn off v-sync

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

		// Get monitor height
		 const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		 m_MonitorWidth = mode->width;
		 m_MonitorHeight = mode->height;

		// Set Frame Buffer Size Callback
		glfwSetFramebufferSizeCallback(m_Window, frame_buffer_size_callback);
		// glfwSetScrollCallback(window, scroll_callback);

		// Culling stuff
		glViewport(0, 0, m_Width, m_Height);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		std::cout << "Display / GLFW initialized\n";

		printf("Created Windows Window: %s (%dx%d)\n", m_Title.c_str(), m_Width, m_Height);

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
