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
			// Push extra layers used
			m_Window->MakeContextCurrent();

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

	return app;
};
