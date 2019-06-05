#pragma once
#include <thread>
#include <fibersapi.h>
#include "threadsafeQueue.h"
#include "jobManager.h"

#define FIBER_COUNT 128
#define FIBER_STACK_SIZE 65536

class JobManager;

namespace threading {
	using Callback = void(*)(void*);
	
	struct atomicCounter {
		volatile ui32 counter = 0;
		inline void decrement() { _InterlockedDecrement(&counter); }
		inline void increment() { _InterlockedIncrement(&counter); }
	};

	struct Fiber
	{
		~Fiber();
		void CreateFromCaller();
		void SwitchTo();

		LPVOID fiber;
		atomicCounter* wakeCondition;
	};
	void fiberCallback(void* data);

	struct Thread
	{
		~Thread();
		void initialize(Callback threadCallback, void* callbackData, ui32 affinity);

		Fiber threadFiber;
		ui32 affinity;
	private:
		std::thread* thread;
	};

	void threadCallback(void* data);
}
