#pragma once

#include "Luxia/Rendering/API/IUIRenderer.h"

namespace Luxia::Rendering::OpenGL {
	class OpenGL_UIRenderer : public IUIRenderer {
		virtual void Init() override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;
		virtual void Render() override;
		virtual void Shutdown() override;
	};

}

