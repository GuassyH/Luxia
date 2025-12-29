#include "Profiler.h"

namespace Talloren::Panels {

	void Profiler::Init(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}
	void Profiler::Render(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// Now how the hell do i implement this
		ImGui::Text("=== Performance Metrics ===");
		ImGui::Text("Time: %.3f ms", Luxia::Core::Time::get().time);
		ImGui::Text("Frames Per Second: %i", Luxia::Core::Time::get().framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / Luxia::Core::Time::get().framerate);
		ImGui::Separator();

		// More detailed profiling would go here

		// Should be a sort of table showing layer times in the form of widths
		/*
		|   |-------|	|
		| |-------|		|
		|    |-|		|
		*/

		ImGui::End();
	}
	void Profiler::Unload(Talloren::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}

	void Profiler::OnEvent(Luxia::Event& e) {}
}