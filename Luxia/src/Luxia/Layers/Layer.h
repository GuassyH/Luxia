#pragma once

#include "Luxia/Core/Core.h"
#include "Luxia/Core/WeakPtrProxy.h"
#include "Luxia/Events/EventDefs.h"
#include "Luxia/Managers/EventManager.h"
#include "Luxia/Managers/ProjectManager.h"
#include "Luxia/Platform/PlatformDefinitions.h"

namespace Luxia
{
	class LUXIA_API Layer {
	public:

		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& event) = 0;

		void SetDeps(
			const std::shared_ptr<Luxia::EventHandler>& handler,
			const std::shared_ptr<Luxia::ProjectManager>& manager,
			const std::shared_ptr<Luxia::Rendering::IRenderer>& m_renderer,
			const std::shared_ptr<Luxia::IWindow>& m_window)
		{ 
		event_handler = handler; project_manager = manager; renderer = m_renderer; 
		asset_manager = project_manager->GetAssetManager();
		scene_manager = project_manager->GetSceneManager();
		window = m_window;
		}

		std::shared_ptr<Luxia::Rendering::IRenderer> GetRenderer() { return renderer.lock(); }

		Luxia::EventHandler& GetEventHandler() { return *event_handler.lock(); }
	protected:
		WeakPtrProxy<Luxia::EventHandler> event_handler;
		WeakPtrProxy<Luxia::ProjectManager> project_manager;
		WeakPtrProxy<Luxia::Rendering::IRenderer> renderer;
		WeakPtrProxy<Luxia::AssetManager> asset_manager;
		WeakPtrProxy<Luxia::SceneManager> scene_manager;
		WeakPtrProxy<Luxia::IWindow> window;
	};
}

