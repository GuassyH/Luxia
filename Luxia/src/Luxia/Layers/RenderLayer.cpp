#include "lxpch.h"
#include "RenderLayer.h"

namespace Luxia::Layers {


	void RenderLayer::OnAttach() {
		LX_CORE_WARN("RenderLayer Attached");

	}
	void RenderLayer::OnDetach() {
		LX_CORE_WARN("RenderLayer Detached");
	}
	void RenderLayer::OnUpdate() {
		cam->UpdateMatrix();

	}
	void RenderLayer::OnRender() {
		for (auto [path, model] : project_manager->GetAssetManager()->GetAssets()) {
			if (model->type == Assets::AssetType::Model) {
				auto ma = std::static_pointer_cast<Assets::ModelAsset>(model);
				if (ma) renderer->RenderModel(ma->model, shader, cam);
			}
		}
	}
	void RenderLayer::OnEvent(Event& e) {

	}
}
