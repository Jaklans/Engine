#pragma once
#include <thread>
#include "core.h"
#include <fibersapi.h>

#define FIBER_COUNT 128
#define FIBER_STACK_SIZE 

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
class jobManager
{
public:
	jobManager(std::thread *threads, ui32 threadCount, ui32 fiberCount, ui32 fiberStackSize);
	~jobManager();
	void runJobs(jobDecl* jobs, ui32 jobCount, atomicCounter** counter);
	void waitForCounter(atomicCounter* counter, ui32 value);
private:
	std::thread* threads;
	ui32 threadCount;
	LPVOID* fibers;
	ui32 fiberCount;
	ui32 fiberStackSize;
};



static jobManager* JobManager;
static threading::tsQueue* JobQueue;