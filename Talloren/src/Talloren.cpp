#include <Luxia.h>
#include <filesystem>

namespace Luxia {
	class ExtraLayer : public Layer {
	public:
		ExtraLayer() = default;
		~ExtraLayer() = default;

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

	virtual void Startup() override {
		// Push extra layers used
		PushLayer(std::make_shared<Luxia::ExtraLayer>());		
		GetWindow()->SetTitle("Island Application");
	}
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();

	// LOAD FIRST
	std::filesystem::path new_path = "E:/BuiltLuxia/NewProject";
	app->GetProjectManager()->OpenProject(new_path);


	return app;
};