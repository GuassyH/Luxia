#include "Log.h"


namespace Luxia
{
	// Static member definitions
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		// Patterns can be changed https://github.com/gabime/spdlog/wiki/Custom-formatting
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Create the core logger
		s_CoreLogger = spdlog::stdout_color_mt("Luxia");
		s_CoreLogger->set_level(spdlog::level::trace);

		// Create the client logger
		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}