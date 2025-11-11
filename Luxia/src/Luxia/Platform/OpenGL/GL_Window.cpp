#include "lxpch.h"
#include "GL_Window.h"

namespace Luxia::Platform::OpenGL {

	GL_Window::GL_Window() = default;
	GL_Window::~GL_Window() = default;

	int GL_Window::Create(const int width, const int height, const std::string& title)
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

		LX_CORE_TRACE("OpenGL Version: {}", (char*)glGetString(GL_VERSION));
		LX_CORE_TRACE("GLSL Version: {}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		LX_CORE_TRACE("Renderer: {}", (char*)glGetString(GL_RENDERER));
		LX_CORE_TRACE("Vendor: {} \n", (char*)glGetString(GL_VENDOR));


		// Get monitor height
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_MonitorWidth = mode->width;
		m_MonitorHeight = mode->height;


		glfwSetWindowUserPointer(m_Window, this);
	
		// Set all the callbacks: ie, when resizing send a WindowResizeEvent to the EventHandler
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) {
				if (action == GLFW_PRESS)
					win->GetEventHandler().PushEvent(std::make_shared<KeyPressEvent>(key));
				else if (action == GLFW_RELEASE)
					win->GetEventHandler().PushEvent(std::make_shared<KeyReleaseEvent>(key));
			}
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) win->GetEventHandler().PushEvent(std::make_shared<MouseMoveEvent>(xpos, ypos));
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
				win->GetEventHandler().PushEvent(std::make_shared<MouseButtonPressEvent>(button));
			else if (action == GLFW_RELEASE)
				win->GetEventHandler().PushEvent(std::make_shared<MouseButtonReleaseEvent>(button));
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) win->GetEventHandler().PushEvent(std::make_shared<MouseScrollEvent>(xoffset, yoffset));
			});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) win->GetEventHandler().PushEvent(std::make_shared<WindowResizeEvent>(width, height));
			});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) win->GetEventHandler().PushEvent(std::make_shared<WindowMoveEvent>(xpos, ypos));
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (win) win->GetEventHandler().PushEvent(std::make_shared<WindowCloseEvent>());
			});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
			GL_Window* win = reinterpret_cast<GL_Window*>(glfwGetWindowUserPointer(window));
			if (!win) return;
			focused == 1 ? win->GetEventHandler().PushEvent(std::make_shared<WindowFocusEvent>())
				: win->GetEventHandler().PushEvent(std::make_shared<WindowLoseFocusEvent>());
			});

		m_PosX = (m_MonitorWidth - m_Width) / 2;
		m_PosY = (m_MonitorHeight - m_Height) / 2;

		// Set Size
		GetEventHandler().PushEvent(std::make_shared<WindowResizeEvent>(m_Width, m_Height));
		GetEventHandler().PushEvent(std::make_shared<WindowMoveEvent>(m_PosX, m_PosY));


		// Culling stuff
		glViewport(0, 0, m_Width, m_Height);
		glEnable(GL_DEPTH_TEST);

		// Cull backface
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Just debug
		LX_CORE_INFO("Created Windows Window: {} ({}x{})", m_Title.c_str(), m_Width, m_Height);

		return 1;
	}
	
	void GL_Window::BeginFrame()
	{
		// Clear the window
		glClearColor(0.3f, 0.5f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_Width, m_Height);

		glfwPollEvents(); // AKA, SEND EVENTS!!!
	}

	void GL_Window::EndFrame()
	{
		// Poll the events and swap the buffer
		glfwSwapBuffers(m_Window);
	}

	void GL_Window::Close() {
		// Destroy the context and glfw, and set running to false
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		running = false; // Dont consume, since other layers might have an onwindowclose function
	}

	void GL_Window::SetTitle(const std::string& title)
	{
		m_Title = title;
		glfwSetWindowTitle(m_Window, m_Title.c_str());
	}

	bool GL_Window::ResizeEvent(WindowResizeEvent& e) {
		// Resize viewport and set the m_width and height params
		glViewport(0, 0, e.GetX(), e.GetY());
		m_Width = e.GetX();
		m_Height = e.GetY();
		return true;
	}

	void GL_Window::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// Check if event is one of the below and call the function
		dispatcher.Dispatch<WindowResizeEvent>(LX_BIND_EVENT_FN(ResizeEvent));
		dispatcher.Dispatch<WindowMoveEvent>(LX_BIND_EVENT_FN(MoveEvent));
		dispatcher.Dispatch<WindowFocusEvent>(LX_BIND_EVENT_FN(FocusEvent));
		dispatcher.Dispatch<WindowLoseFocusEvent>(LX_BIND_EVENT_FN(LoseFocusEvent));
		dispatcher.Dispatch<WindowCloseEvent>(LX_BIND_EVENT_FN(CloseEvent));
	}


}
