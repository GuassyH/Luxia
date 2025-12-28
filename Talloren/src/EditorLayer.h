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
		std::unordered_set<Luxia::GUID> selected_assets;

		bool areNoneSelected = false; // selected == 0
		bool isOneSelected = false; // selected == 1
		bool areMultipleSelected = false; // selected > 1

		std::vector<std::shared_ptr<Talloren::IEditorPanel>> panels;

		void ClearSelected();
		void InsertSelected(Luxia::GUID guid);
		void EraseSelected(Luxia::GUID guid);

		void UpdateSelectedConditions();

		void PushPanel(std::shared_ptr<Talloren::IEditorPanel> m_panel) {
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

