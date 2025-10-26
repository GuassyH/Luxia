#pragma once

#include <Luxia.h>


class IslandLayer : public Luxia::Layer
{
public:
	IslandLayer() = default;
	virtual ~IslandLayer() = default;
	virtual void OnAttach() override {
		LX_WARN("IslandLayer Attached");
	}
	virtual void OnDetach() override {
		LX_WARN("IslandLayer Detached");
	}
	virtual void OnUpdate() override {
		// Update logic here
	}
	virtual void OnEvent() override {
		// Event handling logic here
	}
};