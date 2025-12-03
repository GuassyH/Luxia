#pragma once

#include "Luxia/Core/Core.h"
#include "IUIElement.h"

#include <vector>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Luxia::Rendering {
	class LUXIA_API IUIRenderer {
	public:
		virtual ~IUIRenderer() = default;
		virtual void Init() {}
		virtual void BeginFrame() {}
		virtual void EndFrame() {}
		virtual void RenderElements() {}
		virtual void Shutdown() {}

		void AddUIElement(const std::shared_ptr<IUIElement>& element) {
			m_UIElements.push_back(element);
		}
		void RemoveUIElement(const std::shared_ptr<IUIElement>& element) {
			m_UIElements.erase(std::remove(m_UIElements.begin(), m_UIElements.end(), element), m_UIElements.end());
		}

		ImGuiContext* GetContext() { return m_Context; }
	protected:
		std::vector<std::shared_ptr<IUIElement>> m_UIElements;
		ImGuiContext* m_Context;
	};
}