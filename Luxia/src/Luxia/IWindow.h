#pragma once

#include <iostream>

namespace Luxia
{
	class IWindow
	{
		public:
			virtual ~IWindow() = default;

			virtual int Create(int width, int height, const std::string& title = "Luxia") = 0;
			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;

			virtual void SetTitle(const std::string& title) = 0;
			virtual bool ShouldClose() const = 0;
	};
}
