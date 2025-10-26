#pragma once

#include <Luxia.h>


class IslandLayer : public Luxia::Layer
{
public:
	IslandLayer() = default;
	virtual ~IslandLayer() = default;
	virtual void OnAttach() override {
		printf("Island layer Attached!");
	}
	virtual void OnDetach() override {
		printf("Island layer Detached!");
	}
	virtual void OnUpdate() override {
		// Update logic here
	}
	virtual void OnEvent() override {
		// Event handling logic here
	}
};