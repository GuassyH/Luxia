#pragma once

#include <iostream>
#include <Luxia/Core.h>
#include <Luxia/Events/Event.h>
#include <Luxia/Events/EventHandler.h>

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
	};
}
