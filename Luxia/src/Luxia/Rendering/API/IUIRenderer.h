#pragma once

#include "Luxia/Core/Core.h"
#include "IUIElement.h"

#include <vector>
#include <iostream>

namespace Luxia::Rendering {
	class LUXIA_API IUIRenderer {
	public:
		virtual ~IUIRenderer() = default;
		virtual void Init() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

		void AddUIElement(const std::shared_ptr<IUIElement>& element) {
			m_UIElements.push_back(element);
		}
		void RemoveUIElement(const std::shared_ptr<IUIElement>& element) {
			m_UIElements.erase(std::remove(m_UIElements.begin(), m_UIElements.end(), element), m_UIElements.end());
		}

	protected:
		std::vector<std::shared_ptr<IUIElement>> m_UIElements;
	};
}