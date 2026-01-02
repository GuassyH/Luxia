#include "Profiler.h"
#include "EditorLayer.h"

namespace Editor::Panels {

	static std::unordered_map<std::string, double> layer_times;
	static std::unordered_map<std::string, double> temp_layer_times;
	static bool queue_update = true;
	static float reset_update_timer = 0.0f;

	static bool should_update = false;

	void Profiler::Init(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}
	void Profiler::Render(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {
		ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		name = "Profiler";
		TabPopupContextMenu();

		// Now how the hell do i implement this
		ImGui::Text("=== Performance Metrics ===");
		ImGui::Text("Time: %.3f s", Luxia::Core::Time::get().GetTime());
		ImGui::Text("Frames Per Second: %i", Luxia::Core::Time::get().framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / Luxia::Core::Time::get().framerate);
		ImGui::Separator();

		ImGui::Text("=== Layer Timings ===");
		ImGui::Checkbox("Gather Layer Timings", &should_update);
		
		if(!should_update) {
			ImGui::End();
			return;
		}

		ImVec2 avail = ImGui::GetContentRegionAvail();

		ImGui::BeginTable("LayerTimingsTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders);
		ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Time (ms)", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();

		// For each layer in layer_times, we would get its timing info
		for (const auto& [name, time] : layer_times) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", name.c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("%.3f ms", time * 1000.0);
		}

		ImGui::EndTable();
		
		if (queue_update && reset_update_timer >= 0.2f) { // updates 5 times a second
			layer_times.swap(temp_layer_times);
			temp_layer_times.clear();
			editorLayer->GetEventHandler().PushEvent(std::make_shared<Luxia::ProfilerRequestEvent>(Luxia::Core::Time::get().GetTime()));
			queue_update = false;
			reset_update_timer = 0.0f;
		}
		
		reset_update_timer += Luxia::Core::Time::get().deltaTime;

		ImGui::End();
	}
	void Profiler::Unload(Editor::Layers::EditorLayer* editorLayer, std::shared_ptr<Luxia::Scene> scene) {}

	void Profiler::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::ProfilerResponseEvent>([&](Luxia::ProfilerResponseEvent& event) {
			temp_layer_times[event.GetName()] = event.GetTime();
			return true; // Eat
			});

		dispatcher.Dispatch<Luxia::MessageSentEvent>([&](Luxia::MessageSentEvent& event) {
			if (event.GetMessageStr() == "End of Frame") {
				queue_update = true;
			}
			return false; // Don't eat
			});
	}
}