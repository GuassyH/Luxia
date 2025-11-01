#include <Luxia.h>

namespace Luxia {
	class EditorLayer : public Luxia::Layer{
		public:
			EditorLayer() = default;
			virtual ~EditorLayer() = default;

			// Override Layer methods
			virtual void OnAttach() override {
				LX_WARN("EditorLayer Attached");
			}
			virtual void OnDetach() override {
				LX_WARN("EditorLayer Detached");
			}
			virtual void OnUpdate() override {
				// Update logic here
			}
			virtual void OnRender() override {}
			virtual void OnEvent(Luxia::Event& event) override {
				// Event handling logic here
			}
	};
}

class TallorenApp : public Luxia::Application{
public:
	TallorenApp() = default;
	~TallorenApp() = default;
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();
	app->GetWindow()->SetTitle("Island Application");
	return app;
};