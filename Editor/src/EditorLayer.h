#pragma once
#include "Luxia.h"
#include "IEditorPanel.h"
#include <unordered_set>
#include "ThumbnailManager.h"
#include "SystemFuncs.h"

namespace Editor::Layers {
	class EditorLayer : public Luxia::Layer
	{
	public:
		EditorLayer() { name = "Editor Layer"; }
		~EditorLayer() = default;

		entt::registry editor_reg;
		std::unordered_set<Luxia::GUID> selected_assets;
		std::unordered_map<Luxia::GUID, std::shared_ptr<Luxia::ITexture>> asset_thumbnails;
		std::vector<Luxia::GUID> queued_for_refresh;


		bool areNoneSelected = false; // selected == 0
		bool isOneSelected = false; // selected == 1
		bool areMultipleSelected = false; // selected > 1

		std::unordered_set<std::shared_ptr<Editor::IEditorPanel>> panels;

		void ClearSelected();
		void InsertSelected(Luxia::GUID guid);
		void EraseSelected(Luxia::GUID guid);

		void UpdateSelectedConditions();

		void CreateThumbnails();

		void PushPanel(std::shared_ptr<Editor::IEditorPanel> m_panel) {
			m_panel->Init(this, scene_manager->GetActiveScene()); 
			panels.insert(std::move(m_panel));
		}

		void RemovePanel(std::string& name) {
			for (auto it = panels.begin(); it != panels.end(); ++it) {
				if ((*it)->GetName() == name) {
					(*it)->Unload(this, scene_manager->GetActiveScene());
					panels.erase(it); // erase using iterator
					return;           // exit after removing
				}
			}
		}

		template <typename T>
		bool HasPanel() {
			for (auto& panel : panels) {
				if (std::dynamic_pointer_cast<T>(panel)) {
					return true;
				}
			}
			return false;
		}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnEvent(Luxia::Event& e) override;


		std::shared_ptr<Luxia::ITexture> NoImageTex;
	protected:
		ThumbnailManager thumbnailManager;
		std::shared_ptr<Luxia::ITexture> PlayTex;
		std::shared_ptr<Luxia::ITexture> PauseTex;
	};
}

