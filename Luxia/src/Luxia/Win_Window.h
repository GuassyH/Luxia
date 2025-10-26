#pragma once
#include "IWindow.h"

namespace Luxia::Platform {
	class Win_Window : public Luxia::IWindow {
	public:
		Win_Window();
		~Win_Window();

		int m_Width;
		int m_Height;
		std::string m_Title;

		virtual void Create(int width, int height, const std::string& title) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual bool ShouldClose() const override;
	};
}

