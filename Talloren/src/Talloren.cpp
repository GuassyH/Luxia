#include <Luxia.h>
#include <filesystem>

class TallorenApp : public Luxia::Application {
public:
	TallorenApp() = default;
	~TallorenApp() = default;
};

Luxia::Application* Luxia::CreateApplication(){
	Application* app = new TallorenApp();
	app->GetWindow()->SetTitle("Island Application");

	std::filesystem::path new_path = "E:/BuiltLuxia/NewProject";
	app->GetProjectManager()->OpenProject(new_path);

	return app;
};