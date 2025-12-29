#include "lxpch.h"
#include "ViewportLayer.h"

inline std::vector<Luxia::Rendering::Vertex> quad_verts {
	{{glm::vec3(-1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 0.0f)}},
	{{glm::vec3(1.0f, -1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 0.0f)}},
	{{glm::vec3(-1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(0.0f, 1.0f)}},
	{{glm::vec3(1.0f, 1.0f, 0.0f)}, {glm::vec3(0.0f)}, {glm::vec2(1.0f, 1.0f)}}
};

inline std::vector<unsigned int> quad_inds{
	0, 1, 2,
	1, 3, 2
};

namespace Luxia::Layers {
	void ViewportLayer::OnAttach() {
		LX_CORE_WARN("ViewportLayer Attached");
		fs_shader = Luxia::Platform::Assets::CreateShader(
			"C:/dev/Luxia/assets/shaders/viewport.frag", 
			"C:/dev/Luxia/assets/shaders/viewport.vert");

		fs_quad = Luxia::Mesh(quad_verts, quad_inds);
		fs_quad.CalculateMesh();
	}
	void ViewportLayer::OnDetach() {
		LX_CORE_WARN("ViewportLayer Detached");
	}
	void ViewportLayer::OnUpdate() {

	}
	void ViewportLayer::OnRender() {

	}

	bool ViewportLayer::RenderCameraToVP(Luxia::RenderCameraEvent& e) {
		std::shared_ptr<Luxia::ITexture> output_tex = e.GetTexture();

		// If any of these fail, we don't render to the viewport, and dont consume the event
		if (!e.IsMainCamera() || e.IsEditorCamera()) { LX_CORE_ERROR("ViewportLayer: Camera is editor or not main"); return false; }
		if (!output_tex) { LX_CORE_ERROR("ViewportLayer: Camera Tex is nullptr"); return false; }
		if (!output_tex->IsValid()) { LX_CORE_ERROR("ViewportLayer: Camera Tex is not valid"); return false; }

		renderer->RenderFBO(fs_quad, fs_shader, output_tex);

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
