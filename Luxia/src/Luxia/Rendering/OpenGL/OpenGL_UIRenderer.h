#pragma once

#include "Luxia/Rendering/API/IUIRenderer.h"

namespace Luxia::Rendering::OpenGL {
	class LUXIA_API OpenGL_UIRenderer : public IUIRenderer {
		virtual void Init() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void RenderElements() override;
		virtual void Shutdown() override;
	};

}

