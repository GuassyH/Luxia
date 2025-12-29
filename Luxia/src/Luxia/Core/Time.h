#pragma once

#include <chrono>
#include "Core.h"
#include "Log.h"

namespace Luxia::Core {
	class LUXIA_API Time {
	private:
		float lastFrameTime = 0.0f;
		int nFrames = 0;
		float frameTimer = 0.0f;

		Time() : 
			lastFrameTime(static_cast<float>(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000.0f) 
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
			float current_time = static_cast<float>(
				std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000.0f;
			deltaTime = current_time - lastFrameTime;
			
			frameTimer += deltaTime;
			
			nFrames++;
			if (frameTimer >= 1.0f) {
				framerate = nFrames;
				nFrames = 0;
				frameTimer = 0.0f;
			}


			time = current_time;
			lastFrameTime = current_time;
		}


		float deltaTime = 0.0f;
		float time = 0.0f;
		int framerate = 0;
	};
}