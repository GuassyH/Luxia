#pragma once
#include "Luxia.h"
#include "IEditorPanel.h"

namespace Talloren::Layers {
	class EditorLayer : public Luxia::Layer
	{
	public:
		EditorLayer() = default;
		~EditorLayer() = default;

		entt::registry editor_reg;
		entt::entity selected_entity = entt::entity(0);
		bool is_entity_selected = false;

		std::vector<std::shared_ptr<Talloren::Editor::IEditorPanel>> panels;

		void PushPanel(std::shared_ptr<Talloren::Editor::IEditorPanel> m_panel) {
			m_panel->Init(this, scene_manager->GetActiveScene()); 
			panels.push_back(std::move(m_panel));
		}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;
	};
}

