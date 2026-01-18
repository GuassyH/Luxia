#include <Luxia.h>
#include <filesystem>

#include "Luxia/Core/EntryPoint.h"
#include "EditorLayer.h"

#include "Luxia/Scene/SceneSerializer.h"

namespace Editor {
	class EditorApp : public Luxia::Application {
	public:
		EditorApp() = default;
		~EditorApp() = default;

		virtual void Startup() override {
			// Needed to make sure context is current, otherwise, crash, :(
			m_Window->MakeContextCurrent();

			// Push extra layers used
			PushLayer(std::make_shared<Editor::Layers::EditorLayer>());
			m_Window->SetTitle("Luxia Editor");
			std::filesystem::path iconpath = "C:/dev/Luxia/Editor/resources/LuxiaLogo.png";
			m_Window->SetIcon(iconpath);
		}
	};
}

std::unique_ptr<Luxia::Application> Luxia::CreateApplication() {
	std::unique_ptr<Luxia::Application> app = std::make_unique<Editor::EditorApp>();
	std::filesystem::path new_path = "E:/BuiltLuxia/Sandbox";
	app->GetProjectManager()->OpenProject(new_path);
	app->GetProjectManager()->GetSceneManager()->running = false; // Makes sure that the initial scene isnt loaded by the engine, but rather the editor

	return app;
};
