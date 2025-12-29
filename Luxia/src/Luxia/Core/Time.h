#pragma once

#include <chrono>
#include "Core.h"
#include "Log.h"

namespace Luxia::Core {
	class LUXIA_API Time {
	private:
		double lastFrameTime = 0.0f;
		int nFrames = 0;
		float frameTimer = 0.0f;

		Time() : 
			lastFrameTime(static_cast<double>(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000.0) 
		{}
	public:
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;

		static Time& get() {
			static Time instance;
			return instance;
		}
		void update() {
			// Delta Time Calculation
			double current_time = static_cast<double>(
				std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000.0;
			deltaTime = current_time - lastFrameTime;
			
			frameTimer += deltaTime;
			
			nFrames++;
			if (frameTimer >= 1.0f) {
				framerate = nFrames;
				nFrames = 0;
				frameTimer = 0.0f;
			}


			lastFrameTime = current_time;
		}


		double GetTime() const { return static_cast<double>(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000.0; }

		double GetTimeHP() const {
			return static_cast<double>(
				std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000000.0;
		}

		double deltaTime = 0.0f;
		int framerate = 0;
	};
}