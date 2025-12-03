#include "lxpch.h"
#include "OpenGL_UIRenderer.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <GLFW/glfw3.h>

namespace Luxia::Rendering::OpenGL {
	void OpenGL_UIRenderer::Init() {
		IMGUI_CHECKVERSION();
		m_Context = ImGui::CreateContext();

		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)glfwGetCurrentContext(), true);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingAlwaysTabBar = true;

		ImGui::StyleColorsDark();
		LX_CORE_INFO("OpenGL UI Renderer: Initialized ImGui");
	}

	void OpenGL_UIRenderer::BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGL_UIRenderer::EndFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGL_UIRenderer::RenderElements() {
		for (const auto& element : m_UIElements) {
			element->Render();
		}
	}

	void OpenGL_UIRenderer::Shutdown() {}
}