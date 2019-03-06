#pragma once
#include <thread>
#include "core.h"
#include <fibersapi.h>

struct atomicCounter {
	volatile ui32 counter;
	inline void decrement() { _InterlockedDecrement(&counter); }
};

namespace threading {
	//Threadsafe Lockless Queue to hold ui64 (pointers)
	class tsQueue {
	public:
		tsQueue(ui32 size);
		~tsQueue();
		void Push(void* object);
		void* Pop();
	private:
		volatile ui32 m_head;
		volatile ui32 m_tail;
		ui32 SIZE;
		ui32 SIZE_MASK;
		void** m_buffer;
	};

	struct Thread
	{
		std::thread* threadPtr;
		threading::tsQueue* primaryTaskQueue;
		//Current Task

		int begin();
		int end();

	private:
		bool active = 1;
		Result internalLoop();
	};
}

class ThreadPool
{
public:
	ThreadPool(int threadCount);
	~ThreadPool();
private:
	threading::tsQueue taskQueue;
};
struct jobDecl
{
	void(*task)(void* data);
	void* data;
};
struct jobManager
{
	int i;
	void runJobs(jobDecl* jobs, ui32 jobCount, atomicCounter** counter);
	void waitForCounter(atomicCounter* counter, ui32 value);
};


static jobManager* JobManager;
static threading::tsQueue* JobQueue;