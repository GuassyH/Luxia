#include "lxpch.h"
#include "ViewportLayer.h"
#include "Luxia/Managers/ResourceManager.h"


namespace Luxia::Layers {
	void ViewportLayer::OnAttach() {
		LX_CORE_WARN("ViewportLayer Attached");
		fs_shader = Luxia::Platform::Assets::CreateShader(
			"C:/dev/Luxia/Luxia/resources/shaders/viewport.frag", 
			"C:/dev/Luxia/Luxia/resources/shaders/viewport.vert");

	}
	void ViewportLayer::OnDetach() {
		LX_CORE_WARN("ViewportLayer Detached");

		fs_shader->Delete();
	}
	void ViewportLayer::OnUpdate() {

	}
	void ViewportLayer::OnRender() {

	}

	bool ViewportLayer::RenderCameraToVP(Luxia::RenderCameraEvent& e) {
		std::shared_ptr<Luxia::ITexture> output_tex = e.GetTexture();

		// If any of these fail, we don't render to the viewport, and dont consume the event
		if (!e.IsMainCamera() || e.IsEditorCamera()) { return false; }
		if (!output_tex) { LX_CORE_ERROR("ViewportLayer: Camera Tex is nullptr"); return false; }
		if (!output_tex->IsValid()) { LX_CORE_ERROR("ViewportLayer: Camera Tex is not valid"); return false; }

		renderer->RenderFBO(*ResourceManager::DefaultQuad, fs_shader, output_tex);

		return true;
	}

	void ViewportLayer::OnEvent(Luxia::Event& e) {
		Luxia::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Luxia::RenderCameraEvent>(LX_BIND_EVENT_FN(RenderCameraToVP));

		dispatcher.Dispatch<ProfilerRequestEvent>([&](ProfilerRequestEvent& event) {
			give_profiler_response = true;
			return false; // Check each event type and update input
			});
	}
}
