#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/WeakPtrProxy.h"
#include "Luxia/Events/EventDefs.h"
#include "Luxia/Managers/InputManager.h"
#include "Luxia/Managers/ProjectManager.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia
{
	class Layer {
	public:

		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& event) = 0;

		void SetDeps(std::shared_ptr<EventHandler> handler, std::shared_ptr<ProjectManager> manager, std::shared_ptr<Rendering::IRenderer> m_renderer){
			event_handler = handler;  project_manager = manager; renderer = m_renderer;
		}

		Luxia::EventHandler& GetEventHandler() { return *event_handler.lock(); }
	protected:
		WeakPtrProxy<EventHandler> event_handler;
		WeakPtrProxy<ProjectManager> project_manager;
		WeakPtrProxy<Rendering::IRenderer> renderer;
	};
}

