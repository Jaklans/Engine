#pragma once
#include <chrono>
#include <ctime>

class TimeManager {
public:
	TimeManager() {
		startTime_tp = clock.now();
		currentTime_tp = startTime_tp;
		currentTime = 0;
	}

	void UpdateTime() {
		using namespace std::chrono;

		newTime_tp = clock.now();
		//Get frame duration in ns, convert to float 
		//value in seconds, and store
		tempDeltaT = duration_cast<nanoseconds>(newTime_tp - currentTime_tp).count() / 1000000000.0f;
		_InterlockedExchange(reinterpret_cast<volatile i32*>(&dT), *reinterpret_cast<i32*>(&tempDeltaT));
		_InterlockedExchange64(&currentTime, duration_cast<nanoseconds>(newTime_tp - startTime_tp).count());
		currentTime_tp = newTime_tp;
	};
	inline i64 time() {
		return currentTime;
	};
	inline f32 deltaTime() {
		return dT;
	}
	//Returned as 
	inline void timeString(char* buffer) {
		sprintf_s(buffer, 25, "%02d:%02d:%02d.%03d",
			currentTime / 3600000000000,
			(currentTime / 60000000000) % 60,
			(currentTime / 1000000000) % 60,
			(currentTime / 1000000) % 1000);
	}

private:
	std::chrono::high_resolution_clock clock;
	std::chrono::high_resolution_clock::time_point currentTime_tp;
	std::chrono::high_resolution_clock::time_point newTime_tp;
	std::chrono::high_resolution_clock::time_point startTime_tp;
	f32 tempDeltaT;

	volatile i64 currentTime;
	volatile f32 dT;
};