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
		
		io.DisplaySize = ImVec2(1920, 1080);

		ImGui::StyleColorsDark();

		// Temporary custom colour scheme
		float bright = 1.5f;
		ImGuiStyle* custom_style = &ImGui::GetStyle();
		custom_style->Colors[ImGuiCol_Header] = custom_style->Colors[ImGuiCol_Button];
		custom_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

		custom_style->Colors[ImGuiCol_TitleBgActive] = custom_style->Colors[ImGuiCol_TitleBg];
		custom_style->Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.26f, 1.0f);
		custom_style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f * bright * 0.7f, 0.1f * bright * 0.7f, 0.13f * bright * 0.7f, 1.0f);
		custom_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.1f * bright, 0.1f * bright, 0.13f * bright, 1.0f);

		custom_style->Colors[ImGuiCol_TitleBg] = ImVec4(0.1f * bright * 0.7f, 0.1f * bright * 0.7f, 0.13f * bright * 0.7f, 1.0f);
		custom_style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f * bright * 0.5f, 0.1f * bright * 0.5f, 0.13f * bright * 0.5f, 1.0f);
		custom_style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.349f, 0.416f, 0.580f, 0.7f);

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