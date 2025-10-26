#include <Luxia.h>

#include "IslandLayer.h"

class TallorenApp : public Luxia::Application
{
	public:
		TallorenApp() {}
		~TallorenApp() {}
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();
	app->PushLayer(std::make_shared<IslandLayer>());
	app->GetWindow()->SetTitle("Island Application");
	return app;
};