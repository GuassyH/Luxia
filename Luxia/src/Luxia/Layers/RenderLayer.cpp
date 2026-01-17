#include "lxpch.h"
#include "RenderLayer.h"
#include "Luxia/Scene/Scene.h"
#include "Luxia/Components/MeshRenderer.h"
#include "Luxia/Components/Camera.h"
#include "Luxia/Components/Transform.h"


namespace Luxia::Layers {

	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");
	}
	void RenderLayer::OnDetach() {
		LX_CORE_WARN("RenderLayer Detached");
	}
	void RenderLayer::OnUpdate() {

	}
	void RenderLayer::OnRender() {
		auto scene = project_manager->GetSceneManager()->GetActiveScene();

		if (scene) {
			renderer->RecalculateLightBuffer(scene->GetReg());

			// Do this here so you only calculate all the meshes ONCE a frame
			// Should be done only if a new mesh is added to the scene!
			auto mesh_view = scene->GetEntitiesWith<Luxia::Components::MeshRenderer>();
			renderer->ClearROs();
			for (auto entity : mesh_view) {
				auto& mr = scene->GetFromEntity<Luxia::Components::MeshRenderer>(entity);
				if (mr.transform) {
					renderer->Submit({mr.mesh.get(), mr.material.get(), mr.transform->GetMatrix()});
				}
			}
			
			auto cam = scene->GetMainCamera();
			if (cam)
				PUSH_EVENT(RenderCameraEvent, cam->Render(scene, renderer.lock()), true);
			
		}
	}
	void RenderLayer::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<ProfilerRequestEvent>([&](ProfilerRequestEvent& event) {
			give_profiler_response = true;
			return false; // Check each event type and update input
			});
	}
}
