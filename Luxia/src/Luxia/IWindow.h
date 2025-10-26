#pragma once

#include <iostream>

namespace Luxia
{
	class IWindow
	{
		public:
			virtual ~IWindow() = default;

			virtual void Create(int width, int height, const std::string& title = "Luxia") = 0;
			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;

			virtual bool ShouldClose() const = 0;
	};
}
