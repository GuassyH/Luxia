#include "TestLayer.h"

#include "Application.h"
#include "Events/MouseEvent.h"

void Luxia::Layers::TestLayer::OnUpdate() {
	PUSH_EVENT(Luxia::MouseMoveEvent, 0, 1);
}
