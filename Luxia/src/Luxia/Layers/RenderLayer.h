#pragma once

#include "Layer.h"
#include "Luxia/Assets/ModelAsset.h"


namespace Luxia::Layers
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		RenderLayer() = default;
		virtual ~RenderLayer() = default;
		
		std::shared_ptr<ICamera> cam = Luxia::Platform::Assets::CreateCamera(1920, 1080);
		std::shared_ptr<IShader> shader = Luxia::Platform::Assets::CreateShader("C:/dev/Luxia/assets/shaders/default.frag", "C:/dev/Luxia/assets/shaders/default.vert");

		virtual void OnAttach() override {
			LX_CORE_WARN("RenderLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("RenderLayer Detached");
		}
		virtual void OnUpdate() override {
			// Rendering logic here
			cam->UpdateMatrix();
		}
		virtual void OnRender() override {
			// TEMP, SHOULD BE SCENE.GETSCENEASSETS OR SO
			for (auto [path, model] : project_manager->GetAssetManager()->GetAssets()) {
				if (model->type == Assets::AssetType::Model) {
					auto ma = std::static_pointer_cast<Assets::ModelAsset>(model);
					if(ma) renderer->RenderModel(ma->model, shader, cam);
				}
			}
		}
		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);

			// Handle rendering events here
			// LX_CORE_TRACE("RenderLayer received event: {}", e.GetDebug());
		}
	};
}