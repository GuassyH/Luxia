#pragma once

#include "Luxia/Core/Core.h"

namespace Luxia {
	class LUXIA_API IUIElement {
	public:
		virtual ~IUIElement() = default;
		virtual void Render() = 0;
	};
}