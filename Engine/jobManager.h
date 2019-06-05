#pragma once
#include "threadsafeQueue.h"
#include "threadingConstructs.h"

namespace threading {
	struct Thread;
	class tsQueue;
	struct atomicCounter;
}
struct App;


struct job
{
	void(*task)(void* data);
	void* data;
};

struct FiberNode {
	threading::Fiber fiber;
	FiberNode* next = 0;
};

enum priority {
	HIGH_PRIORITY,
	REGULAR_PRIORITY,
	LOW_PRIORITY
};

class JobManager
{
public:
	JobManager(ui32 fiberCount, ui32 fiberStackSize, job startupJob);
	~JobManager();

	void runJob(job& job, priority jobPriority = REGULAR_PRIORITY);

	void runJobs(job* jobs, ui32 jobCount, priority jobPriority = REGULAR_PRIORITY);
	//void runJobs(jobDecl* jobs, ui32 jobCount, atomicCounter** counter, priority jobPriority);

	void waitForCounter(threading::atomicCounter* counter, ui32 value);

	threading::Fiber* getNewFiber();
	bool getJob(job& jobRef);

	inline bool shutingDown() { return shuttingDown; };

private:
	threading::Thread* threads;
	ui32 threadCount;

	threading::Fiber* fibers;
	ui32 fiberCount;

	threading::atomicCounter checkingFibers;
	FiberNode* inactiveFibers;
	FiberNode* waitingFibers;
	ui32 fiberStackSize;

	threadsafe::Queue<job> highPrioJobQ;
	threadsafe::Queue<job> jobQ;
	threadsafe::Queue<job> lowPrioJobQ;
	bool shuttingDown;
};