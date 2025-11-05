#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "Core.h"

namespace Luxia
{
	class LUXIA_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

// Macros, in code you can call LX_INFO("message"); and itll log to the core logger!
#define LX_CORE_TRACE(...)    ::Luxia::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LX_CORE_INFO(...)     ::Luxia::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LX_CORE_WARN(...)     ::Luxia::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LX_CORE_ERROR(...)    ::Luxia::Log::GetCoreLogger()->error(__VA_ARGS__)

// Same but for client logger
#define LX_TRACE(...)         ::Luxia::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LX_INFO(...)          ::Luxia::Log::GetClientLogger()->info(__VA_ARGS__)
#define LX_WARN(...)          ::Luxia::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LX_ERROR(...)         ::Luxia::Log::GetClientLogger()->error(__VA_ARGS__)
