#include <Luxia.h>

class TallorenApp : public Luxia::Application
{
	public:
		TallorenApp() {}
		~TallorenApp() {}
};

Luxia::Application* Luxia::CreateApplication(){
	return new TallorenApp();
};