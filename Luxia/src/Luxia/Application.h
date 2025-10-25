#pragma once

#include "Core.h"

namespace Luxia
{
	class LUXIA_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
	};

	// Each client defines this function; editor, runtime, etc.
	Application* CreateApplication();
}

