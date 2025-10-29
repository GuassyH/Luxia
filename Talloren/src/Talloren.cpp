#include <Luxia.h>


class IslandLayer : public Luxia::Layer
{
	public:
		IslandLayer() = default;
		virtual ~IslandLayer() = default;

		// Override Layer methods
		virtual void OnAttach() override {
			LX_WARN("IslandLayer Attached");
		}
		virtual void OnDetach() override {
			LX_ERROR("IslandLayer Detached");
		}
		virtual void OnUpdate() override {
			// Update logic here
		}
		virtual void OnEvent(Luxia::Event& event) override {
			// Event handling logic here
		}
};

class TallorenApp : public Luxia::Application
{
public:
	TallorenApp() = default;
	~TallorenApp() = default;
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();
	app->PushLayer(std::make_shared<IslandLayer>());
	app->GetWindow()->SetTitle("Island Application");
	return app;
};