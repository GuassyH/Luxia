#include <Luxia.h>
#include <filesystem>

namespace Luxia {
	class ExtraLayer : public Layer {
	public:
		ExtraLayer() = default;
		~ExtraLayer() = default;
		ExtraLayer(
			const std::shared_ptr<EventHandler>& handler,
			const std::shared_ptr<ProjectManager>& manager,
			const std::shared_ptr<Rendering::IRenderer>& m_renderer)
			: Layer(handler, manager, m_renderer) {
		}

		virtual void OnAttach() override { 
			LX_CORE_WARN("ExtraLayer Attached");
		}
		virtual void OnDetach() override {
			LX_CORE_WARN("ExtraLayer Detached");
		}
		virtual void OnUpdate() override {}
		virtual void OnRender() override {}
		virtual void OnEvent(Event& event) override {}
	};
}


class TallorenApp : public Luxia::Application {
public:
	TallorenApp() = default;
	~TallorenApp() = default;
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();

	// LOAD FIRST
	std::filesystem::path new_path = "E:/BuiltLuxia/NewProject";
	app->GetProjectManager()->OpenProject(new_path);
	app->GetWindow()->SetTitle("Island Application");


	// PUSH LAST
	app->PushLayer(std::make_shared<ExtraLayer>(app->GetEventHandler(), app->GetProjectManager(), app->GetRenderer()));

	return app;
};