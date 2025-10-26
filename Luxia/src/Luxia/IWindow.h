#pragma once
class IWindow
{
	public:
		IWindow() {}
		virtual ~IWindow() {}
		virtual void Render() {}

		int m_Width = 800;
		int m_Height = 600;
		const char* m_Title = "Luxia Window";
};

