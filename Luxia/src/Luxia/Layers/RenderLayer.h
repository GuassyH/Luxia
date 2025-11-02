#pragma once

#include "Layer.h"
#include "Luxia/PlatformDefinitions.h"

namespace Luxia::Layers
{
	class LUXIA_API RenderLayer : public Layer
	{
	public:
		std::shared_ptr<Rendering::VAO> vao = nullptr;
		std::shared_ptr<Rendering::VBO> vbo = nullptr;
		std::shared_ptr<Rendering::EBO> ebo = nullptr;

		std::vector<float> vertices{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		std::vector<int> indices{
			0, 1, 2
		};

		RenderLayer() = default;
		virtual ~RenderLayer() = default;
		
		virtual void OnAttach() override {
			LX_CORE_WARN("RenderLayer Attached");

			vao = Rendering::CreateVAO();
			vbo = Rendering::CreateVBO();
			ebo = Rendering::CreateEBO();

			vbo->BindBufferData(sizeof(float) * vertices.size(), vertices.data());

			vao->LinkAttrib(0, 3, GL_FLOAT, sizeof(float) * 3, 0);

			ebo->BindBufferData(indices.size() * sizeof(int), indices.data());

			vbo->Unbind();
			vao->Unbind();
			ebo->Unbind();

			if (!vao) { LX_CORE_ERROR("VAO IS NULL"); return; }
			if (!vbo) { LX_CORE_ERROR("VBO IS NULL"); return; }
			if (!ebo) { LX_CORE_ERROR("EBO IS NULL"); return; }

		}
		virtual void OnDetach() override {
			LX_CORE_WARN("RenderLayer Detached");
		}
		virtual void OnUpdate() override {
			// Rendering logic here
		}
		virtual void OnRender() override {
			vao->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		}
		virtual void OnEvent(Event& e) override {
			EventDispatcher dispatcher(e);

			// Handle rendering events here
			// LX_CORE_TRACE("RenderLayer received event: {}", e.GetDebug());
		}
	};
}