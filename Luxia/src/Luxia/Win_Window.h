#pragma once
#include "IWindow.h"

class Win_Window : public IWindow{
public:
	Win_Window();
	virtual ~Win_Window();
	virtual void Render() override;
};

